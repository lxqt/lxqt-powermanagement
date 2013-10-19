/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * Razor - a lightweight, Qt based, desktop toolset
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

#include "lidwatcherd.h"
#include "../config/constants.h"

LidWatcherd::LidWatcherd(QObject *parent) :
    QObject(parent),
    mSettings("razor-autosuspend")
{
    bool performFirstRunCheck = mSettings.value(FIRSTRUNCHECK_KEY, false).toBool();
    if (performFirstRunCheck)
    {
        mSettings.setValue(FIRSTRUNCHECK_KEY, false);
    }

    connect(&mLid, SIGNAL(changed(bool)), this, SLOT(lidChanged(bool)));
}

LidWatcherd::~LidWatcherd()
{
}

void LidWatcherd::lidChanged(bool closed)
{
    if (closed)
    {
        doAction(mSettings.value(LIDCLOSEDACTION_KEY).toInt());
    }
}

void LidWatcherd::doAction(int action)
{
    switch (action)
    {
    case SLEEP:
        mRazorPower.suspend();
        break;
    case HIBERNATE:
        mRazorPower.hibernate();
        break;
    case POWEROFF:
        mRazorPower.shutdown();
        break;
    }
}
