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

#include <QCoreApplication>
#include <KIdleTime>
#include <QDebug>

IdlenessWatcher::IdlenessWatcher(QObject* parent):
    Watcher(parent)
{
    qDebug() << "Starting idlenesswatcher";
    
    mIdleWatcher = mIdleBacklightWatcher = mBacklightActualValue = -1;
    mBacklight = new LXQt::Backlight(this);

    connect(KIdleTime::instance(),
            static_cast<void (KIdleTime::*)(int)>(&KIdleTime::timeoutReached),
            this,
            &IdlenessWatcher::timeoutReached);

    if(mBacklight->isBacklightAvailable()) {
        connect(KIdleTime::instance(),
                &KIdleTime::resumingFromIdle,
                this,
                &IdlenessWatcher::resumingFromIdle);
    }
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
    if(mPSettings.isIdlenessWatcherEnabled()) {
        int timeout = 1000 * mPSettings.getIdlenessTimeSecs();
        mIdleWatcher = KIdleTime::instance()->addIdleTimeout(timeout);
    }
    if(mBacklight->isBacklightAvailable()) {
        if(mPSettings.isIdlenessBacklightWatcherEnabled()) {
            QTime time = mPSettings.getIdlenessBacklightTime();
            mIdleBacklightWatcher = KIdleTime::instance()->addIdleTimeout((time.second() + time.minute() * 60) * 1000);
        }
    }
}

void IdlenessWatcher::timeoutReached(int identifier)
{
    if(identifier == mIdleWatcher) {
        doAction(mPSettings.getIdlenessAction());
    }
    if(identifier == mIdleBacklightWatcher) {
        mBacklightActualValue = mBacklight->getBacklight();
        mBacklight->setBacklight((float)mBacklightActualValue * (float)(mPSettings.getBacklight())/100.0f);
        KIdleTime::instance()->catchNextResumeEvent();
    }
}

void IdlenessWatcher::resumingFromIdle()
{
    if(mBacklight->isBacklightAvailable() && mBacklightActualValue > -1) {
        if(mPSettings.isIdlenessBacklightWatcherEnabled()) {
            mBacklight->setBacklight(mBacklightActualValue);
            mBacklightActualValue = -1;
        }
    }
}

void IdlenessWatcher::onSettingsChanged()
{
    KIdleTime::instance()->removeAllIdleTimeouts();
    setup();
}
