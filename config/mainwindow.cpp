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
#include "mainwindow.h"
#include "lidwatchersettings.h"
#include "batterywatchersettings.h"
#include "idlenesswatchersettings.h"

MainWindow::MainWindow(QWidget *parent) :
    LxQt::ConfigDialog(tr("LXQt Powermanagement Configuration"), new LxQt::Settings("lxqt-autosuspend"), parent)
{
    LidSettings *lidSettings = new LidSettings(mSettings, this);
    addPage(lidSettings, tr("Lid"), "laptop-lid");
    connect(this, SIGNAL(reset()), lidSettings, SLOT(loadSettings()));

    BatteryWatcherSettings* powerLowSettings = new BatteryWatcherSettings(mSettings, this);
    addPage(powerLowSettings, tr("Battery"), "battery");
    connect(this, SIGNAL(reset()), powerLowSettings, SLOT(loadSettings()));

    IdleSettings* idleSettings = new IdleSettings(mSettings, this);
    addPage(idleSettings, tr("Idle"), "idle");
    connect(this, SIGNAL(reset()), idleSettings, SLOT(loadSettings()));

    emit reset();
}
