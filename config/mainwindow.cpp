/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXQt - a lightweight, Qt based, desktop toolset
 * https://lxqt.org
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
#include "powerkeyssettings.h"

#include <LXQt/Globals>

MainWindow::MainWindow(QWidget *parent) :
    LXQt::ConfigDialog(tr("Power Management Settings"), new PowerManagementSettings(parent))
{
    setButtons(QDialogButtonBox::Apply | QDialogButtonBox::Close | QDialogButtonBox::Reset);
    enableButton(QDialogButtonBox::Apply, false); // disable Apply button until something is changed

    BatteryWatcherSettings* batteryWatcherSettings = new BatteryWatcherSettings(this);
    addPage(batteryWatcherSettings, tr("Battery"), QSL("battery"));
    connect(this, &MainWindow::reset, batteryWatcherSettings, &BatteryWatcherSettings::loadSettings);
    connect(batteryWatcherSettings, &BatteryWatcherSettings::settingsChanged, this, [this] {
        enableButton(QDialogButtonBox::Apply, true);
    });

    LidWatcherSettings *lidwatcherSettings = new LidWatcherSettings(this);
    addPage(lidwatcherSettings, tr("Lid"), QSL("laptop-lid"));
    connect(this, &MainWindow::reset, lidwatcherSettings, &LidWatcherSettings::loadSettings);
    connect(lidwatcherSettings, &LidWatcherSettings::settingsChanged, this, [this] {
        enableButton(QDialogButtonBox::Apply, true);
    });

    IdlenessWatcherSettings* idlenessWatcherSettings = new IdlenessWatcherSettings(this);
    addPage(idlenessWatcherSettings, tr("Idle"), (QStringList() << QSL("user-idle") << QSL("user-away")));
    connect(this, &MainWindow::reset, idlenessWatcherSettings, &IdlenessWatcherSettings::loadSettings);
    connect(idlenessWatcherSettings, &IdlenessWatcherSettings::settingsChanged, this, [this] {
        enableButton(QDialogButtonBox::Apply, true);
    });

    PowerKeysSettings* powerKeysSettings = new PowerKeysSettings(this);
    addPage(powerKeysSettings, tr("Power keys"), QSL("system-shutdown"));
    connect(this, &MainWindow::reset, powerKeysSettings, &PowerKeysSettings::loadSettings);
    connect(powerKeysSettings, &PowerKeysSettings::settingsChanged, this, [this] {
        enableButton(QDialogButtonBox::Apply, true);
    });

    connect(this, &MainWindow::reset, [this]() {
        enableButton(QDialogButtonBox::Apply, false);
    });
    connect(this, &LXQt::ConfigDialog::clicked, [=, this] (QDialogButtonBox::StandardButton btn) {
        if (btn == QDialogButtonBox::Apply) {
            batteryWatcherSettings->saveSettings();
            lidwatcherSettings->saveSettings();
            idlenessWatcherSettings->saveSettings();
            powerKeysSettings->saveSettings();
            enableButton(QDialogButtonBox::Apply, false);
        }
    });
}
