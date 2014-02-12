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
        mSettings("lxqt-autosuspend"),
        mBatterywatcherd(0),
        mLidwatcherd(0),
        mIdlenesswatcherd(0)
{
    connect(&mSettings, SIGNAL(settingsChanged()), this, SLOT(settingsChanged()));
    settingsChanged();
}

PowerManagementd::~PowerManagementd()
{
}

void PowerManagementd::settingsChanged()
{
    qDebug() << "PowerManagementd::settingsChanged...";
    
    startStopWatcher(mBatterywatcherd, mSettings.value(ENABLE_BATTERY_WATCHER, true).toBool()); 
    startStopWatcher(mLidwatcherd, mSettings.value(ENABLE_LID_WATCHER, true).toBool());
}

template<class Watcher> void PowerManagementd::startStopWatcher(Watcher*& watcher, bool enabled)
{
    if (watcher == 0 && enabled) 
    {
        watcher = new Watcher();
    }
    else if (watcher != 0 && !enabled) {
        watcher->deleteLater();
        watcher = 0;
    }
}



