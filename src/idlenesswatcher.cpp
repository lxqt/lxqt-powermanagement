/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXQt - a lightweight, Qt based, desktop toolset
 * https://lxqt.org
 *
 * Copyright (C) 2013  Alec Moskvin <alecm@gmx.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#include "idlenesswatcher.h"

#include <QGuiApplication>
#include <QTimer>
#include <KIdleTime>
#include <Solid/Device>
#include <Solid/Battery>
#include <KWindowSystem>
#include <KX11Extras>
#include <KWindowInfo>
#include <QDebug>
#include <LXQt/lxqtnotification.h>
#include <QObject>
#include <xcb/dpms.h>
#include <xcb/screensaver.h>

IdlenessWatcher::IdlenessWatcher(QObject* parent):
    Watcher(parent)
{
    qDebug() << "Starting idlenesswatcher";

    mIdleACWatcher = mIdleBatteryWatcher = mIdleBacklightWatcher = mBacklightActualValue = -1;
    mBacklight = nullptr;
    mDischarging = false;

    connect(KIdleTime::instance(),
            static_cast<void (KIdleTime::*)(int,int)>(&KIdleTime::timeoutReached),
            this,
            &IdlenessWatcher::timeoutReached);

    // Init backlight control
    connect(KIdleTime::instance(),
            &KIdleTime::resumingFromIdle,
            this,
            &IdlenessWatcher::resumingFromIdle);
    const QList<Solid::Device> devices = Solid::Device::listFromType(Solid::DeviceInterface::Battery, QString());

    for (Solid::Device device : devices)
    {
        Solid::Battery *battery = device.as<Solid::Battery>();
        if (battery->type() == Solid::Battery::PrimaryBattery) {
            connect(battery, &Solid::Battery::chargeStateChanged, this, &IdlenessWatcher::onBatteryChanged);
            mDischarging |= battery->chargeState() == Solid::Battery::ChargeState::Discharging;
        }
    }

    connect(&mPSettings, &LXQt::Settings::settingsChanged, this, &IdlenessWatcher::onSettingsChanged);

    // retrieve DPMS timeouts
    mDpmsStandby = mDpmsSuspend = mDpmsOff = 0;
    if (QGuiApplication::platformName() == QStringLiteral("xcb")) {
        if (auto x11NativeInterface = qGuiApp->nativeInterface<QNativeInterface::QX11Application>()) {
            xcb_connection_t* c = x11NativeInterface->connection();
            xcb_dpms_get_timeouts_cookie_t cookie = xcb_dpms_get_timeouts(c);
            xcb_dpms_get_timeouts_reply_t* reply = xcb_dpms_get_timeouts_reply(c, cookie, nullptr);
            if (reply) {
                mDpmsStandby = reply->standby_timeout;
                mDpmsSuspend = reply->suspend_timeout;
                mDpmsOff = reply->off_timeout;
                free(reply);
            }
        }
    }

    setup();
}

IdlenessWatcher::~IdlenessWatcher()
{
    setDpmsTimeouts(true);
    KIdleTime::instance()->removeAllIdleTimeouts();
}

void IdlenessWatcher::setDpmsTimeouts(bool restore) {
    if (QGuiApplication::platformName() == QStringLiteral("xcb")) {
        if (auto x11NativeInterface = qGuiApp->nativeInterface<QNativeInterface::QX11Application>()) {
            xcb_connection_t* c = x11NativeInterface->connection();
            if (restore) {
                xcb_dpms_set_timeouts(c, mDpmsStandby, mDpmsSuspend, mDpmsOff);
                xcb_screensaver_suspend(c, 0); // WARNING: This is not documented but works.
            }
            else {
                xcb_dpms_set_timeouts(c, 0, 0, 0);
                xcb_screensaver_suspend(c, XCB_SCREENSAVER_SUSPEND);
            }
        }
    }
}

void IdlenessWatcher::setup()
{
    if(mPSettings.isIdlenessWatcherEnabled()) {
        QTime ACtime = mPSettings.getIdlenessACTime();
        int ACmsecs = (ACtime.second() + ACtime.minute() * 60) * 1000;
        mIdleACWatcher = KIdleTime::instance()->addIdleTimeout(ACmsecs);

        if(mPSettings.isACMonitorOffEnabled()) {
            QTime ACMonitortime = mPSettings.getMonitorACIdleTime();
            int ACMonitormsecs = (ACMonitortime.second() + ACMonitortime.minute() * 60) * 1000;
            if (ACmsecs == ACMonitormsecs) {
            ACmsecs += 20; //  20 msecs more
            }
            mIdleACMonitorWatcher = KIdleTime::instance()->addIdleTimeout(ACMonitormsecs);
            }

        QTime BATtime = mPSettings.getIdlenessBatteryTime();
        int BATmsecs = (BATtime.second() + BATtime.minute() * 60) * 1000;
        // to get sure times are NOT the same ones... Is that needed? They are never together active
        if (BATmsecs == ACmsecs) {
            BATmsecs -= 10; // just 10 msecs less... ;)
        }
        mIdleBatteryWatcher = KIdleTime::instance()->addIdleTimeout(BATmsecs);

        if(mPSettings.isBatMonitorOffEnabled()) {
            QTime BATMonitortime = mPSettings.getMonitorBatIdleTime();
            int BATMonitormsecs = (BATMonitortime.second() + BATMonitortime.minute() * 60) * 1000;

            if (BATmsecs == BATMonitormsecs) {
            ACmsecs += 20; //  20 msecs more
            }
            mIdleBatMonitorWatcher = KIdleTime::instance()->addIdleTimeout(BATMonitormsecs);
            }

        // Enable backlight control:
        if(mPSettings.isIdlenessBacklightWatcherEnabled() &&
            (
                !mPSettings.isIdlenessBacklightOnBatteryDischargingEnabled()
                  ||
                (
                    mPSettings.isIdlenessBacklightOnBatteryDischargingEnabled()
                      &&
                    mDischarging
                )
            )
          ) {
            QTime time = mPSettings.getIdlenessBacklightTime();
            int milliseconds = (time.second() + time.minute() * 60) * 1000;
            if(milliseconds < 1000)
                milliseconds = 1000;
            mIdleBacklightWatcher = KIdleTime::instance()->addIdleTimeout(milliseconds);
        }

        // override DPMS settings
        setDpmsTimeouts(false);
    }
    else {
        // restore DPMS settings
        setDpmsTimeouts(true);
    }
}

void IdlenessWatcher::timeoutReached(int identifier,int /*msec*/)
{
    if (mPSettings.isIdlenessWatcherPaused()) {
        QTimer::singleShot(0, this, [] {
            KIdleTime::instance()->simulateUserActivity();
        });
        return;
    }

    // check if disable Idleness when fullscreen is enabled
    if (mPSettings.isDisableIdlenessWhenFullscreenEnabled()) {
        WId w = KX11Extras::activeWindow();
        KWindowInfo info(w, NET::WMState);
        if (info.hasState(NET::FullScreen)) {
            QTimer::singleShot(0, this, [] {
                KIdleTime::instance()->simulateUserActivity();
            });
            return;
        }
    }

    if(identifier == mIdleACWatcher) {
        if (mDischarging) {
            return;
        }
        doAction(mPSettings.getIdlenessACAction());
        return;
    }

    if(identifier == mIdleACMonitorWatcher) {
    if (mDischarging) {
        return;
    }
    mPower.doAction(LXQt::Power::PowerMonitorOff);
    return;
    }

    if(identifier == mIdleBatteryWatcher) {
        if (!mDischarging) {
            return;
        }
        doAction(mPSettings.getIdlenessBatteryAction());
        return;
    }

    if(identifier == mIdleBatMonitorWatcher) {
    if (!mDischarging) {
        return;
    }
    mPower.doAction(LXQt::Power::PowerMonitorOff);
    return;
    }

    if(identifier == mIdleBacklightWatcher && mBacklightActualValue < 0) {
        if(mBacklight == nullptr) {
            mBacklight = new LXQt::Backlight();
            connect(mBacklight, &QObject::destroyed, this, [this](QObject *) {mBacklight = nullptr;} );
        }

        //LXQt::Notification::notify(QStringLiteral("IdlenessWatcher::timeoutReached"),
        //    mBacklight->isBacklightAvailable() ?
        //     QStringLiteral("").setNum(mBacklightActualValue):QStringLiteral("Error!!"));

        mBacklightActualValue = mBacklight->getBacklight();
        if(mBacklight->isBacklightAvailable() && !mBacklight->isBacklightOff())
            mBacklight->setBacklight((float)mBacklightActualValue * (float)(mPSettings.getBacklight())/100.0f);

        KIdleTime::instance()->removeIdleTimeout(mIdleBacklightWatcher);
        KIdleTime::instance()->catchNextResumeEvent();

        mBacklight->deleteLater();
    }
}

void IdlenessWatcher::resumingFromIdle()
{
    if(mBacklightActualValue > -1) {
        if(mPSettings.isIdlenessBacklightWatcherEnabled()) {
            if(mBacklight == nullptr) {
                mBacklight = new LXQt::Backlight();
                connect(mBacklight, &QObject::destroyed, this, [this](QObject *) {mBacklight = nullptr;} );
            }

            if(mBacklight->isBacklightAvailable() && !mBacklight->isBacklightOff())
                mBacklight->setBacklight(mBacklightActualValue);
            mBacklightActualValue = -1;
            onSettingsChanged();

            mBacklight->deleteLater();
        }
    }
}

void IdlenessWatcher::onBatteryChanged(int, const QString &)
{
    const QList<Solid::Device> devices = Solid::Device::listFromType(Solid::DeviceInterface::Battery, QString());

    mDischarging = false;

    for (Solid::Device device : devices) {
        Solid::Battery *battery = device.as<Solid::Battery>();
        if (battery->type() == Solid::Battery::PrimaryBattery) {
            mDischarging |= battery->chargeState() == Solid::Battery::ChargeState::Discharging;
        }
    }

    onSettingsChanged();
}

void IdlenessWatcher::onSettingsChanged()
{
    KIdleTime::instance()->removeAllIdleTimeouts();
    mIdleACWatcher = mIdleBatteryWatcher = mIdleBacklightWatcher = -1;
    setup();
}
