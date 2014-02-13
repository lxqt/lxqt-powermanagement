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
#include <QTimer>
#include <QCoreApplication>
#include <lxqt/lxqtautostartentry.h>
#include <qt4/QtCore/qprocess.h>
#include <qt4/QtCore/qtextstream.h>
#include <QDebug>
#include "lidwatcherd.h"
#include "../config/common.h"

LidWatcherd::LidWatcherd(QObject *parent) :
QObject(parent),
mSettings("lxqt-autosuspend")
{
    qDebug() << "Starting lidwatcher";
    connect(&mLid, SIGNAL(changed(bool)), this, SLOT(lidChanged(bool)));
}

LidWatcherd::~LidWatcherd()
{
    qDebug() << "Stopping lidwatcher";
}

void LidWatcherd::lidChanged(bool closed)
{
    qDebug() << "LidWatcherd#lidChanged: closed=" << closed;
    if (closed)
    {
        doAction(action());
    }
}

void LidWatcherd::doAction(int action)
{
    qDebug() << "LidWatcherd.doAction, action=" << action;
    switch (action)
    {
        case SLEEP:
            mLxQtPower.suspend();
            break;
        case HIBERNATE:
            mLxQtPower.hibernate();
            break;
        case POWEROFF:
            mLxQtPower.shutdown();
            break;
        case LOCK:
            // FIXME Do something;
            break;
    }
}

int LidWatcherd::action()
{
    PowerManagementSettings settings;
    if (settings.isEnableExtMonLidClosedActions() && externalMonitorPlugged())
    {
        if (mLid.onBattery())
        {
            return settings.getLidClosedExtMonAction();
        }
        else
        {
            return settings.getLidClosedExtMonAcAction();
        }
    }
    else
    {
        if (mLid.onBattery())
        {
            return settings.getLidClosedAction();
        }
        else
        {
            return settings.getLidClosedAcAction(); 
        }
    }
}

bool LidWatcherd::externalMonitorPlugged()
{
    int monitorCount = 0;

    QProcess xrandr(this);
    xrandr.start("xrandr", QIODevice::ReadOnly);
    xrandr.waitForFinished(1000);

    if (xrandr.exitCode() != 0)
    {
        return false; // Well, what to do?
    }

    QTextStream xrandr_stdout(&xrandr);
    while (!xrandr_stdout.atEnd())
    {
        QString line = xrandr_stdout.readLine();
        qDebug() << ">>" << line;
        if (line.indexOf(" connected", 0) > -1)
        {
            monitorCount++;
        }
    }

    qDebug() << "monitorCount: " << monitorCount;

    return monitorCount >= 2;
}
