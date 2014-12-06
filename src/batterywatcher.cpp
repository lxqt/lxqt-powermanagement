/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXDE-Qt - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2012 Razor team
 * Authors:
 *   Christian Surlykke <christian@surlykke.dk>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#include <QDebug>
#include <QTime>
#include <QTimer>

#include <LXQt/Notification>
#include <LXQt/AutostartEntry>
#include <LXQt/Power>

#include "batterywatcher.h"
#include "../config/powermanagementsettings.h"

BatteryWatcher::BatteryWatcher(QObject *parent) : Watcher(parent),
    mBattery(0), mTrayIcon(0)
{
    mBattery = new Battery(this);
    if (!mBattery->haveBattery())
    {
        LxQt::Notification::notify(tr("No battery!"),
                                   tr("LXQt could not find data about any battery - monitoring disabled"),
                                   "lxqt-powermanagement");
    }

    connect(mBattery, SIGNAL(chargeStateChange(float,Battery::State)), this, SLOT(batteryChanged()));

    QList<Battery*> batteries;
    batteries.append(mBattery);
    mBatteryInfoDialog = new BatteryInfoDialog(batteries);

    connect(&mSettings, SIGNAL(settingsChanged()), this, SLOT(settingsChanged()));
    connect(LxQt::Settings::globalSettings(), SIGNAL(iconThemeChanged()), this, SLOT(settingsChanged()));
    settingsChanged();
    batteryChanged();
}

BatteryWatcher::~BatteryWatcher()
{
    if (mTrayIcon)
    {
        mTrayIcon->deleteLater();
    }

    if (mBatteryInfoDialog)
    {
        mBatteryInfoDialog->deleteLater();
    }
}

void BatteryWatcher::batteryChanged()
{
    static QTime actionTime;
    static LxQt::Notification *notification = 0;

    qDebug() <<  "BatteryChanged"
             <<  "state:"       << mBattery->stateAsText
             <<  "chargeLevel:" << mBattery->chargeLevel
             <<  "actionTime:"  << actionTime;

    bool powerLowActionRequired =
            mBattery->state == Battery::Discharging &&
            mBattery->chargeLevel < mSettings.getPowerLowLevel() &&
            mSettings.getPowerLowAction() > 0;

    if (powerLowActionRequired)
    {
        if (actionTime.isNull())
        {
            actionTime = QTime::currentTime().addMSecs(mSettings.getPowerLowWarningTime()*1000);
        }

        if (notification == 0)
        {
            notification = new LxQt::Notification(tr("Power low!"), this);
            notification->setTimeout(2000);
        }

        int milliSecondsToAction = QTime::currentTime().msecsTo(actionTime);

        if (milliSecondsToAction > 0)
        {
            int secondsToAction = milliSecondsToAction/1000;
            switch (mSettings.getPowerLowAction())
            {
            case LxQt::Power::PowerSuspend:
                notification->setBody(tr("Suspending in %1 seconds").arg(secondsToAction));
                break;
            case LxQt::Power::PowerHibernate:
                notification->setBody(tr("Hibernating in %1 seconds").arg(secondsToAction));
                break;
            case LxQt::Power::PowerShutdown:
                notification->setBody(tr("Shutting down in %1 seconds").arg(secondsToAction));
                break;
            }

            notification->update();

            QTimer::singleShot(200, this, SLOT(batteryChanged()));
        }
        else
        {
            doAction(mSettings.getPowerLowAction());
        }
    }
    else
    {
        if (!actionTime.isNull())
        {
            actionTime = QTime();
        }

        if (notification)
        {
            delete notification;
            notification = 0;
        }
    }
}

void BatteryWatcher::settingsChanged()
{
    if (mTrayIcon != 0 && !mSettings.isShowIcon())
    {
        mTrayIcon->hide();
        mTrayIcon->deleteLater();
        mIconProducer->deleteLater();
        mTrayIcon = 0;
    }

    if (mTrayIcon == 0 && mSettings.isShowIcon())
    {
        mTrayIcon = new TrayIcon(mBattery, this);
        connect(mTrayIcon, SIGNAL(toggleShowInfo()), mBatteryInfoDialog, SLOT(toggleShow()));
        mTrayIcon->show();
    }
}

