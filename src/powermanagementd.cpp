/* 
 * File:   powermanagementd.cpp
 * Author: Christian Surlykke
 * 
 * Created on 9. februar 2014, 16:15
 */
#include <QDebug>

#include "powermanagementd.h"
#include "../config/common.h"
#include "idlenesswatcherd.h"
#include "lidwatcherd.h"
#include "batterywatcherd.h"

PowerManagementd::PowerManagementd() : 
        mBatterywatcherd(0),
        mLidwatcherd(0),
        mIdlenesswatcherd(0),
        mSettings()
 {
    connect(&mSettings, SIGNAL(settingsChanged()), this, SLOT(settingsChanged()));
    settingsChanged();
}

PowerManagementd::~PowerManagementd()
{
}

void PowerManagementd::settingsChanged()
{
    if (mSettings.isBatteryWatcherEnabled() && !mBatterywatcherd) 
    { 
        mBatterywatcherd = new BatteryWatcherd(this);
    }
    else if (mBatterywatcherd && ! mSettings.isBatteryWatcherEnabled())
    {
        mBatterywatcherd->deleteLater();
        mBatterywatcherd = 0;
    }

    if (mSettings.isLidWatcherEnabled() && !mLidwatcherd) 
    { 
        mLidwatcherd = new LidWatcherd(this);
    }
    else if (mLidwatcherd && ! mSettings.isLidWatcherEnabled())
    {
        mLidwatcherd->deleteLater();
        mLidwatcherd = 0;
    }

}