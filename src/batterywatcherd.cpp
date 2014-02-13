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
#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include <QtCore/QCoreApplication>
#include <lxqt/lxqtautostartentry.h>

#include "batterywatcherd.h"
#include "../config/common.h"

BatteryWatcherd::BatteryWatcherd(QObject *parent) :
    QObject(parent),
    mBatteryInfo(),
    mBattery(),
    mTrayIcon(0),
    mSettings()
{
    if (!mBattery.haveBattery())
    {
        LxQt::Notification::notify(tr("No battery!"),
                                  tr("LxQt Powermanagement could not find data about any battery - battery will not be monitored"),
                                  "lxqt-powermanagement");
    }

    connect(&mBattery, SIGNAL(batteryChanged()), this, SLOT(batteryChanged()));
    connect(&mSettings, SIGNAL(settingsChanged()), this, SLOT(settingsChanged()));
    connect(LxQt::Settings::globalSettings(), SIGNAL(iconThemeChanged()), this, SLOT(settingsChanged()));
    settingsChanged();
    batteryChanged();
}

BatteryWatcherd::~BatteryWatcherd()
{
    if (mTrayIcon)
    {
        delete mTrayIcon;
    }
}

void BatteryWatcherd::batteryChanged()
{
    static QTime actionTime;
    static LxQt::Notification *notification = 0;

    qDebug() <<  "BatteryChanged"
             <<  "discharging:"  << mBattery.discharging() 
             << "chargeLevel:" << mBattery.chargeLevel() 
             << "powerlow:"    << mBattery.powerLow() 
             << "actionTime:"  << actionTime;


    if (mBattery.powerLow() && mSettings.getPowerLowAction() > 0)
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
            case SLEEP:
                notification->setBody(tr("Sleeping in %1 seconds").arg(secondsToAction));
                break;
            case HIBERNATE:
                notification->setBody(tr("Hibernating in %1 seconds").arg(secondsToAction));
                break;
            case POWEROFF:
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

    mBatteryInfo.updateInfo(&mBattery);

    mTrayIcon->update(mBattery.discharging(), mBattery.chargeLevel(), mSettings.getPowerLowLevel());
}

void BatteryWatcherd::doAction(int action)
{
    LxQt::Power power;

    switch (action)
    {
    case SLEEP:
        power.suspend();
        break;
    case HIBERNATE:
        power.hibernate();
        break;
    case POWEROFF:
        power.shutdown();
        break;
    }
}

void BatteryWatcherd::settingsChanged()
{
    if (mTrayIcon != 0 && !mTrayIcon->isProperForCurrentSettings())
    {
        mTrayIcon->hide(); 
        mTrayIcon->deleteLater();
        mTrayIcon = 0;
    }
   
    if (mTrayIcon == 0) 
    {
        IconNamingScheme *iconNamingScheme = 0;
        if (mSettings.isUseThemeIcons() && (iconNamingScheme = IconNamingScheme::getNamingSchemeForCurrentIconTheme()))
        {
            mTrayIcon = new TrayIconTheme(iconNamingScheme, this);
        }
        else 
        {
            mTrayIcon = new TrayIconBuiltIn(this);
        }
    
        bool discharging = mBattery.state() == 2;
        qDebug() << "updating trayicon: " << discharging << mBattery.chargeLevel() << mSettings.getPowerLowLevel();
        
        connect(mTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(showBatteryInfo()));
        mTrayIcon->update(discharging, mBattery.chargeLevel(), mSettings.getPowerLowLevel());
        mTrayIcon->show();
    }
}

void BatteryWatcherd::showBatteryInfo()
{
    if (mBatteryInfo.isVisible())
    {
        mBatteryInfo.close();
    }
    else
    {
        mBatteryInfo.open();
    }
}