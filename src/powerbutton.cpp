/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXQt - a lightweight, Qt based, desktop toolset
 * https://lxqt.org
 *
 * Copyright: 2020 LXQt team
 * Authors:
 *   Pedro L. Lucas <selairi@gmail.com>
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

#include <QDBusConnection>
#include <QDBusReply>
#include <QObject>
#include <QDebug>

#include "powerbutton.h"

#include <LXQt/Globals>
#include <LXQt/Notification>
#include <lxqt-globalkeys.h>
#include <unistd.h>

PowerButton::PowerButton(QObject *parent) : Watcher(parent)
{

    QDBusInterface manager(QSL("org.freedesktop.login1"),
                           QSL("/org/freedesktop/login1"),
                           QSL("org.freedesktop.login1.Manager"),
                           QDBusConnection::systemBus(), this);


    // QString powerKeyAction = mLogindInterface->property("HandlePowerKey").toString();
    //LXQt::Notification::notify(powerKeyAction, powerKeyAction);

    mFd = manager.call(QL1S("Inhibit"), QL1S("handle-power-key"), QL1S("lxqt-powermanager"), QL1S("lxqt-powermanager controls power key"), QL1S("block"));

    QDBusError error = manager.lastError();
    qDebug() << QStringLiteral("PowerButton") << error.name() << error.message() ;

    //LXQt::Notification::notify(QL1S("powermanager"), QStringLiteral("Fd %1").arg(mFd.value().fileDescriptor()));

    mKeyPowerButton = GlobalKeyShortcut::Client::instance()->addAction(QStringLiteral("XF86PowerOff"), QStringLiteral("/powermanager/keypoweroff"), tr("Power off key action"), this);

    if (mKeyPowerButton) {
        //connect(m_keyVolumeUp, &GlobalKeyShortcut::Action::registrationFinished, this, &LXQtVolume::shortcutRegistered);
        connect(mKeyPowerButton, SIGNAL(activated()), this, SLOT(handleShortcutPoweroff()));
    }
}


PowerButton::~PowerButton()
{
    close(mFd.value().fileDescriptor());
}


void PowerButton::handleShortcutPoweroff()
{
    //mLogindInterface->call(QL1S("Suspend"), false);
    //QDBusError error = mLogindInterface->lastError();
    //qDebug() << QStringLiteral("PowerButton") << error.name() << error.message() ;
    PowerManagementSettings mSettings;
    qDebug() << "Suspend" << mSettings.getPowerKeyAction();
    doAction(mSettings.getPowerKeyAction());
}
