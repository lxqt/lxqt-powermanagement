/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXDE-Qt - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2011 Razor team
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

#include <QDBusConnection>
#include <QDebug>
#include <lxqt/lxqtapplication.h>
#include "lidwatcherd.h"

int main(int argc, char *argv[])
{

    LxQt::Application a(argc, argv);
//    TRANSLATE_APP;

    // To ensure only one instance of lidwatcherd is running we register as a DBus service and refuse to run
    // if not able to do so.
    // We do not register any object as we don't have any dbus-operations to expose.
    if (! QDBusConnection::sessionBus().registerService("org.lxqt.lidwatcherd"))
    {
        qWarning() << "Unable to register 'org.lxqt.lidwatcherd' service - is another instance of lxqt-autosuspend running?";
        return 1;
    }
    else
    {
        LidWatcherd lidWatcherd;
        return a.exec();
    }
}
