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
#ifndef COMMON_H
#define COMMON_H 

#include <QComboBox>
#include <QString>
#include <lxqt/lxqtsettings.h>

#define ENABLE_BATTERY_WATCHER "enableBatteryWatcher"
#define ENABLE_LID_WATCHER "enableLidWatcher"
#define ENABLE_IDLENESS_WATCHER "enableIdlenessWatcher"
#define LIDCLOSEDACTION_KEY "lidClosedAction"
#define LIDCLOSED_AC_ACTION_KEY "lidClosedAction_on_ac"
#define LIDCLOSED_EXT_MON_ACTION_KEY "lidClosedAction_extern_monitor"
#define LIDCLOSED_EXT_MON_AC_ACTION_KEY "lidClosedAction_extern_monitor_ac"
#define ENABLE_EXT_MON_LIDCLOSED_ACTIONS "enable_external_monitor_lidClosed_actions"
#define POWERLOWACTION_KEY "powerLowAction"
#define POWERLOWWARNING_KEY "powerLowWarning"
#define POWERLOWLEVEL_KEY "powerLowLevel"
#define SHOWTRAYICON_KEY "showTrayIcon"
#define USETHEMEICONS_KEY "useThemeIcons"
#define FIRSTRUNCHECK_KEY "performFirstRunCheck"

enum
{
    NOTHING,
    SLEEP,
    HIBERNATE,
    POWEROFF,
    LOCK
};

void fillComboBox(QComboBox* comboBox);

void loadValueFromSettings(LxQt::Settings *settings, QComboBox* comboBox, const QString & settingsKey, int defaultValue);

int currentValue(QComboBox *comboBox);

#endif // COMMON_H
