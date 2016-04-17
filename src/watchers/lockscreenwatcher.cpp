/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXQt - a lightweight, Qt based, desktop toolset
 * http://lxqt.org
 *
 * Copyright: 2016 LXQt team
 * Authors:
 *   Paulo Lieuthier <paulolieuthier@gmail.com>
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

#include "lockscreenwatcher.h"
#include <LXQt/ScreenSaver>
#include <QDBusReply>
#include <QDebug>
#include <QEventLoop>
#include <unistd.h>

LogindProvider::LogindProvider(QObject *parent) :
    mInterface(QStringLiteral("org.freedesktop.login1"),
            QStringLiteral("/org/freedesktop/login1"),
            QStringLiteral("org.freedesktop.login1.Manager"),
            QDBusConnection::systemBus())
{
    connect(&mInterface, SIGNAL(PrepareForSleep(bool)),
            this, SIGNAL(aboutToSleep(bool)));
}

LogindProvider::~LogindProvider()
{
    release();
}

bool LogindProvider::isActive()
{
    return mInterface.isValid();
}

bool LogindProvider::inhibit()
{
    QDBusReply<QDBusUnixFileDescriptor> reply = mInterface.call(
            QStringLiteral("Inhibit"),
            QStringLiteral("sleep"),
            QStringLiteral("LXQt Power Management"),
            QStringLiteral("Start screen locker before sleep."),
            QStringLiteral("delay"));
    if (!reply.isValid())
    {
        qDebug() << "LockScreenWatcher: " << reply.error();
        return false;
    }
    else
        mFileDescriptor = reply.value();

    return true;
}

void LogindProvider::release()
{
    if (mFileDescriptor.isValid())
    {
        ::close(mFileDescriptor.fileDescriptor());
        mFileDescriptor.setFileDescriptor(-1);
    }
}

LockScreenWatcher::LockScreenWatcher(QObject *parent)
{
    mProvider = new LogindProvider(this);
    if (!mProvider->isActive())
    {
        // mProvider->deleteLater();
        // mProvider = new SomethingElse();
    }

    if (mProvider)
    {
        connect(mProvider, &LockScreenProvider::aboutToSleep,
                this, &LockScreenWatcher::aboutToSleep);
        mProvider->inhibit();
    }
}

LockScreenWatcher::~LockScreenWatcher()
{
}

void LockScreenWatcher::aboutToSleep(bool beforeSleep)
{
    if (beforeSleep)
    {
        LXQt::ScreenSaver mScreenSaver;
        QEventLoop loop;
        connect(&mScreenSaver, &LXQt::ScreenSaver::activated,
                &loop, &QEventLoop::quit);
        mScreenSaver.lockScreen();
        loop.exec();
    }
}
