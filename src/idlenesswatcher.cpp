/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXDE-Qt - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
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

#include <QCoreApplication>
#include <KIdleTime>
#include <QDebug>

IdlenessWatcher::IdlenessWatcher(QObject* parent):
    Watcher(parent),
    mBatteryId(),
    mAcId()
{
    qDebug() << "Starting idlenesswatcher";

    connect(KIdleTime::instance(),
            static_cast<void (KIdleTime::*)(int)>(&KIdleTime::timeoutReached),
            this,
            &IdlenessWatcher::timeoutReached);

    connect(&mPSettings, &LXQt::Settings::settingsChanged,
            this, &IdlenessWatcher::onSettingsChanged);

    setup();
}

IdlenessWatcher::~IdlenessWatcher()
{
    KIdleTime::instance()->removeAllIdleTimeouts();
}

void IdlenessWatcher::setup()
{
    if (mPSettings.isIdlenessWatcherEnabled()) {
        int timeout = 1000 * mPSettings.getIdlenessTimeSecs();
        mBatteryId = KIdleTime::instance()->addIdleTimeout(timeout);
    }
    if (mPSettings.isIdlenessAcWatcherEnabled()) {
        int timeout = 1000 * mPSettings.getIdlenessAcTimeSecs();
        mAcId = KIdleTime::instance()->addIdleTimeout(timeout);
    }
}

void IdlenessWatcher::timeoutReached(int identifier)
{
    if (identifier == mBatteryId && mLid.onBattery()) {
        doAction(mPSettings.getIdlenessAction());
    }
    if (identifier == mAcId && !mLid.onBattery()) {
        doAction(mPSettings.getIdlenessAcAction());
    }
}

void IdlenessWatcher::onSettingsChanged()
{
    KIdleTime::instance()->removeAllIdleTimeouts();
    setup();
}
