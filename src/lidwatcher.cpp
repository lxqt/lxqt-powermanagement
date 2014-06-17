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
#include <QProcess>
#include <QDebug>
#include <QDBusInterface>
#include <QDBusReply>

#include "lidwatcher.h"
#include "../config/powermanagementsettings.h"

LidWatcher::LidWatcher(QObject *parent) : Watcher(parent)
{
    qDebug() << "Starting lidwatcher";
    inhibitSystemdLogin();
    connect(&mLid, SIGNAL(changed(bool)), this, SLOT(lidChanged(bool)));
}

LidWatcher::~LidWatcher(){
    qDebug() << "Stopping lidwatcher";
}

void LidWatcher::lidChanged(bool closed)
{
    qDebug() << "LidWatcherd#lidChanged: closed=" << closed;
    if (closed)
    {
        doAction(action());
    }
}

int LidWatcher::action()
{
    if (mSettings.isEnableExtMonLidClosedActions() && externalMonitorPlugged())
    {
        if (mLid.onBattery())
        {
            return mSettings.getLidClosedExtMonAction();
        }
        else
        {
            return mSettings.getLidClosedExtMonAcAction();
        }
    }
    else
    {
        if (mLid.onBattery())
        {
            return mSettings.getLidClosedAction();
        }
        else
        {
            return mSettings.getLidClosedAcAction(); 
        }
    }
}

bool LidWatcher::externalMonitorPlugged()
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

void LidWatcher::inhibitSystemdLogin()
{
    QDBusInterface manager("org.freedesktop.login1",
                           "/org/freedesktop/login1",
                           "org.freedesktop.login1.Manager",
                           QDBusConnection::systemBus(), this);
    QDBusReply<QDBusUnixFileDescriptor> reply = manager.call("Inhibit", "handle-lid-switch", "lxqt-powermanagment", "LidWatcher is in da house!", "block");
    if (reply.isValid())
    {
        logindLock = reply.value();
        qDebug() << "Inhibit got:" << logindLock.fileDescriptor();
    }
    else
    {
        qDebug() << "Error from inhibit:" << reply.error();
    }
}

/*gdbus introspect --system --dest org.freedesktop.login1 --object-path /org/freedesktop/login1
node /org/freedesktop/login1 {
  interface org.freedesktop.login1.Manager {
    methods:
      Inhibit(in  s what,
              in  s who,
              in  s why,
              in  s mode,
              out h fd);
      ListInhibitors(out a(ssssuu) inhibitors);
      ...
    signals:
      PrepareForShutdown(b active);
      PrepareForSleep(b active);
      ...
    properties:
      readonly s BlockInhibited = '';
      readonly s DelayInhibited = '';
      readonly t InhibitDelayMaxUSec = 5000000;
      readonly b PreparingForShutdown = false;
      readonly b PreparingForSleep = false;
      ...
  };
  ...
};*/
