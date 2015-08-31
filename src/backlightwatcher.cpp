/*
* Copyright (c) 2015 Dmitry Kontsevoy
*
* This file is part of the LXQt project. <http://lxqt.org>
* It is distributed under the LGPL 2.1 or later license.
* Please refer to the LICENSE file for a copy of the license, and
* the AUTHORS file for copyright and authorship information.
*/

#include <QDebug>
#include <Solid/Device>

#include "backlightwatcher.h"

BacklightWatcher::BacklightWatcher(QObject *parent) : QObject(parent)
{
    QList<Solid::Device> devices = Solid::Device::listFromType(Solid::DeviceInterface::Battery, QString());

    for (auto device: devices)
    {
        auto battery = device.as<Solid::Battery>();
        if (battery->type() == Solid::Battery::PrimaryBattery)
        {
            mBatteries << battery;
            connect(battery, &Solid::Battery::chargeStateChanged, this, &BacklightWatcher::updateBacklightLevel);
        }
    }

    connect(&mSettings, SIGNAL(settingsChanged()), this, SLOT(updateBacklightLevel()));

    updateBacklightLevel();
}

BacklightWatcher::~BacklightWatcher()
{
}

void BacklightWatcher::updateBacklightLevel()
{
    if(mBacklight.isSupported()){
        bool discharging = true;

        for (auto battery: mBatteries)
        {
            discharging &= (battery->chargeState() == Solid::Battery::Discharging);
        }

        int level = 100;
        if(discharging){
            level = mSettings.getBacklightBatteryLevel();
        } else {
            level = mSettings.getBacklightACLevel();
        }

        long actual_level = mBacklight.levelMax()*level/100;
        qDebug() << "Setting backlight level: " << actual_level;

        mBacklight.setLevel(actual_level);
    }
}

