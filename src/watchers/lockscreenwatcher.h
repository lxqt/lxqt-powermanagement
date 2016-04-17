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

#ifndef LOCKSCREENWATCHER_H
#define LOCKSCREENWATCHER_H

#include "watcher.h"
#include "../config/powermanagementsettings.h"

#include <QDBusInterface>
#include <QDBusUnixFileDescriptor>

class LockScreenProvider;
class LogindProvider;
class ConsoleKitProvider;

/*
 * Provider interface
 */
class LockScreenProvider : public QObject
{
    Q_OBJECT

public:
    virtual ~LockScreenProvider() {}
    virtual bool isActive() = 0;
    virtual bool inhibit() = 0;
    virtual void release() = 0;

signals:
    void aboutToSleep(bool);
};

/*
 * Provider for logind
 */
class LogindProvider : public LockScreenProvider
{
    Q_OBJECT

public:
    explicit LogindProvider(QObject *parent = nullptr);
    virtual ~LogindProvider();

    bool isActive() override;
    bool inhibit() override;
    void release() override;

private:
    QDBusInterface mInterface;
    QDBusUnixFileDescriptor mFileDescriptor;
};

/*
 * Provider for ConsoleKit
 */
class ConsoleKitProvider : public LockScreenProvider
{
    Q_OBJECT

public:
    explicit ConsoleKitProvider(QObject *parent = nullptr);
    virtual ~ConsoleKitProvider();

    bool isActive() override;
    bool inhibit() override;
    void release() override;

private:
    bool mMethodInhibitNotPresent;
    QDBusInterface mInterface;
    QDBusUnixFileDescriptor mFileDescriptor;
};

/*
 * Actual watcher
 */
class LockScreenWatcher : public Watcher
{
    Q_OBJECT

public:
    explicit LockScreenWatcher(QObject *parent = nullptr);
    virtual ~LockScreenWatcher();

private slots:
    void aboutToSleep(bool beforeSleep);

private:
    LockScreenProvider *mProvider;
    PowerManagementSettings mSettings;
};

#endif
