/*
 * File:   powermanagementd.cpp
 * Author: Christian Surlykke
 *
 * Created on 9. februar 2014, 16:15
 */
#include <QDebug>
#include <QStringList>

#include "powermanagementd.h"
#include "../config/powermanagementsettings.h"
#include "idlenesswatcher.h"
#include "lidwatcher.h"
#include "batterywatcher.h"

PowerManagementd::PowerManagementd() :
        mBatterywatcherd(0),
        mLidwatcherd(0),
        mIdlenesswatcherd(0),
        mSettings()
 {
    connect(&mSettings, SIGNAL(settingsChanged()), this, SLOT(settingsChanged()));
    settingsChanged();

    if (mSettings.isPerformFirstRunCheck())
    {
        mNotification.setSummary(tr("Power Management"));
        mNotification.setBody(tr("You are running LXQt Power Management for the first time.\nYou can configure it from settings... "));
        mNotification.setActions(QStringList() << tr("Configure..."));
        mNotification.setTimeout(10000);
        connect(&mNotification, SIGNAL(actionActivated(int)), SLOT(runConfigure()));
        mNotification.update();

        mSettings.setPerformFirstRunCheck(false);
    }
}

PowerManagementd::~PowerManagementd()
{
}

void PowerManagementd::settingsChanged()
{
    if (mSettings.isBatteryWatcherEnabled() && !mBatterywatcherd)
    {
        mBatterywatcherd = new BatteryWatcher(this);
    }
    else if (mBatterywatcherd && ! mSettings.isBatteryWatcherEnabled())
    {
        mBatterywatcherd->deleteLater();
        mBatterywatcherd = 0;
    }

    if (mSettings.isLidWatcherEnabled() && !mLidwatcherd)
    {
        mLidwatcherd = new LidWatcher(this);
    }
    else if (mLidwatcherd && ! mSettings.isLidWatcherEnabled())
    {
        mLidwatcherd->deleteLater();
        mLidwatcherd = 0;
    }

    if (mSettings.isIdlenessWatcherEnabled() && !mIdlenesswatcherd)
    {
        mIdlenesswatcherd = new IdlenessWatcher(this);
    }
    else if (mIdlenesswatcherd && !mSettings.isIdlenessWatcherEnabled())
    {
        mIdlenesswatcherd->deleteLater();
        mIdlenesswatcherd = 0;
    }

}

void PowerManagementd::runConfigure()
{
    mNotification.close();
    QProcess::startDetached("lxqt-config-powermanagement");
}
