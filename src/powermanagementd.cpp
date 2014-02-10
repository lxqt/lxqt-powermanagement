/* 
 * File:   powermanagementd.cpp
 * Author: christian
 * 
 * Created on 9. februar 2014, 16:15
 */

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
    settingsChanged();
}

PowerManagementd::~PowerManagementd()
{
}

void PowerManagementd::settingsChanged()
{
    startStopWatcher(mBatterywatcherd, mSettings.value(ENABLE_BATTERY_WATCHER, true).toBool());
    startStopWatcher(mLidwatcherd, mSettings.value(ENABLE_LID_WATCHER, true).toBool());
    startStopWatcher(mIdlenesswatcherd, mSettings.value(ENABLE_IDLENESS_WATCHER, true).toBool());

}

template<class Watcher> void PowerManagementd::startStopWatcher(Watcher*& watcher, bool enabled)
{
    if (watcher == 0 && enabled) 
    {
        watcher = new Watcher();
    }
    else if (watcher != 0 && enabled) {
        watcher->deleteLater();
        watcher = 0;
    }
}

