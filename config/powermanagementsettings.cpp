/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXQt - a lightweight, Qt based, desktop toolset
 *
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

#include <QDebug>

#include <LXQt/Power>

#include "powermanagementsettings.h"

// the pause state is only for the tray icon and is not saved
bool PowerManagementSettings::mIdlenessWatcherPaused = false;

namespace PowerManagementSettingsConstants
{
    const QString RUN_CHECK_LEVEL { QSL("runCheckLevel") };
    const QString ENABLE_BATTERY_WATCHER_KEY { QSL("enableBatteryWatcher") };
    const QString ENABLE_LID_WATCHER_KEY { QSL("enableLidWatcher") };
    const QString ENABLE_IDLENESS_WATCHER_KEY { QL1S("enableIdlenessWatcher") };
    const QString ENABLE_IDLENESS_BACKLIGHT_WATCHER_KEY { QL1S("enableIdlenessBacklightWatcher") };
    const QString LID_CLOSED_ACTION_KEY { QL1S("lidClosedAction") };
    const QString LID_CLOSED_AC_ACTION_KEY { QL1S("lidClosedAcAction") };
    const QString LID_CLOSED_EXT_MON_ACTION_KEY { QL1S("lidClosedExtMonAction") };
    const QString LID_CLOSED_EXT_MON_AC_ACTION_KEY { QL1S("lidClosedExtMonAcAction") };
    const QString ENABLE_EXT_MON_LIDCLOSED_ACTIONS_KEY { QL1S("enableExtMonLidClosedActions") };
    const QString POWER_LOW_ACTION_KEY { QL1S("powerLowAction") };
    const QString POWER_LOW_WARNING_KEY { QL1S("powerLowWarning") };
    const QString POWER_LOW_LEVEL_KEY { QL1S("powerLowLevel") };
    const QString SHOW_ICON_KEY { QL1S("showIcon") };
    const QString USE_THEME_ICONS_KEY { QL1S("useThemeIcons") };
    const QString IDLENESS_AC_ACTION_KEY { QL1S("idlenessACAction") };
    const QString IDLENESS_AC_TIME { QL1S("idlenessACTime") };
    const QString IDLENESS_BATTERY_ACTION_KEY { QL1S("idlenessBatteryAction") };
    const QString IDLENESS_BATTERY_TIME { QL1S("idlenessBatteryTime") };
    const QString IDLENESS_BACKLIGHT_TIME { QL1S("idlenessTime") };
    const QString IDLENESS_BACKLIGHT { QL1S("backlightIdleness") };
    const QString IDLENESS_BACKLIGHT_ON_BATTERY_DISCHARGING { QL1S("backlightIdlenessOnBatteryDischarging") };
    const QString DISABLE_IDLENESS_WHEN_FULLSCREEN { QL1S("disableIdlenessWhenFullscreen") };
    const QString POWER_KEY_ACTION { QL1S("powerKeyAction") };
    const QString SUSPEND_KEY_ACTION { QL1S("suspendKeyAction") };
    const QString HIBERNATE_KEY_ACTION { QL1S("hibernateKeyAction") };
}

using namespace PowerManagementSettingsConstants;

PowerManagementSettings::PowerManagementSettings(QObject* parent) : LXQt::Settings(QSL("lxqt-powermanagement"), parent)
{
}

PowerManagementSettings::~PowerManagementSettings()
{
}

int PowerManagementSettings::getRunCheckLevel()
{
    return value(RUN_CHECK_LEVEL, 0).toInt();
}

void PowerManagementSettings::setRunCheckLevel(int newLevel)
{
    setValue(RUN_CHECK_LEVEL, newLevel);
}

bool PowerManagementSettings::isBatteryWatcherEnabled()
{
    return value(ENABLE_BATTERY_WATCHER_KEY, true).toBool();
}

void PowerManagementSettings::setBatteryWatcherEnabled(bool batteryWatcherEnabled)
{
    setValue(ENABLE_BATTERY_WATCHER_KEY, batteryWatcherEnabled);
}

int PowerManagementSettings::getPowerLowAction()
{
    return value(POWER_LOW_ACTION_KEY, -1).toInt();
}

void PowerManagementSettings::setPowerLowAction(int powerLowAction)
{
    setValue(POWER_LOW_ACTION_KEY, powerLowAction);
}

int PowerManagementSettings::getPowerLowLevel()
{
    return value(POWER_LOW_LEVEL_KEY, 5).toInt();
}

void PowerManagementSettings::setPowerLowLevel(int powerLowLevel)
{
    setValue(POWER_LOW_LEVEL_KEY, powerLowLevel);
}

int PowerManagementSettings::getPowerLowWarningTime()
{
    return value(POWER_LOW_WARNING_KEY, 30).toInt();
}

void PowerManagementSettings::setPowerLowWarningTime(int powerLowWarningTime)
{
    setValue(POWER_LOW_WARNING_KEY, powerLowWarningTime);
}

bool PowerManagementSettings::isShowIcon()
{
    return value(SHOW_ICON_KEY, true).toBool();
}

void PowerManagementSettings::setShowIcon(bool showIcon)
{
    setValue(SHOW_ICON_KEY, showIcon);
}

bool PowerManagementSettings::isUseThemeIcons()
{
    return value(USE_THEME_ICONS_KEY, false).toBool();
}

void PowerManagementSettings::setUseThemeIcons(bool useThemeIcons)
{
    setValue(USE_THEME_ICONS_KEY, useThemeIcons);
}

bool PowerManagementSettings::isLidWatcherEnabled()
{
    return value(ENABLE_LID_WATCHER_KEY, true).toBool();
}

void PowerManagementSettings::setLidWatcherEnabled(bool lidWatcherEnabled)
{
    setValue(ENABLE_LID_WATCHER_KEY, lidWatcherEnabled);
}

int PowerManagementSettings::getLidClosedAcAction()
{
    return value(LID_CLOSED_AC_ACTION_KEY, -1).toInt();
}

void PowerManagementSettings::setLidClosedAcAction(int lidClosedAcAction)
{
    setValue(LID_CLOSED_AC_ACTION_KEY, lidClosedAcAction);
}

int PowerManagementSettings::getLidClosedAction()
{
    return value(LID_CLOSED_ACTION_KEY, -1).toInt();
}

void PowerManagementSettings::setLidClosedAction(int lidClosedAction)
{
    setValue(LID_CLOSED_ACTION_KEY, lidClosedAction);
}

int PowerManagementSettings::getLidClosedExtMonAcAction()
{
    return value(LID_CLOSED_EXT_MON_AC_ACTION_KEY, -1).toInt();
}

void PowerManagementSettings::setLidClosedExtMonAcAction(int lidClosedExtMonAcAction)
{
    setValue(LID_CLOSED_EXT_MON_AC_ACTION_KEY, lidClosedExtMonAcAction);
}

int PowerManagementSettings::getLidClosedExtMonAction()
{
    return value(LID_CLOSED_EXT_MON_ACTION_KEY, -1).toInt();
}

void PowerManagementSettings::setLidClosedExtMonAction(int lidClosedExtMonAction)
{
    setValue(LID_CLOSED_EXT_MON_ACTION_KEY, lidClosedExtMonAction);
}

bool PowerManagementSettings::isEnableExtMonLidClosedActions()
{
    return value(ENABLE_EXT_MON_LIDCLOSED_ACTIONS_KEY, 0).toBool();
}

void PowerManagementSettings::setEnableExtMonLidClosedActions(bool enableExtMonLidClosedActions)
{
    setValue(ENABLE_EXT_MON_LIDCLOSED_ACTIONS_KEY, enableExtMonLidClosedActions);
}


bool PowerManagementSettings::isIdlenessWatcherEnabled()
{
    return value(ENABLE_IDLENESS_WATCHER_KEY, false).toBool();
}

void PowerManagementSettings::setIdlenessWatcherEnabled(bool idlenessWatcherEnabled)
{
    setValue(ENABLE_IDLENESS_WATCHER_KEY, idlenessWatcherEnabled);
}

int PowerManagementSettings::getIdlenessACAction()
{
    // defaults to nothing (-1) and eventually load legacy value
    int oldValue = value(QL1S("idlenessAction"), -1).toInt();
    return value(IDLENESS_AC_ACTION_KEY, oldValue).toInt();
}

void PowerManagementSettings::setIdlenessACAction(int idlenessAction)
{
    setValue(IDLENESS_AC_ACTION_KEY, idlenessAction);
}

QTime PowerManagementSettings::getIdlenessACTime()
{
    // 1. The default value is 15 minutes (the legacy key is also read);
    // 2. Intervals greater than or equal to one hour aren't supported; and
    // 3. Intervals smaller than one minute are prevented.
    int oldValue = qBound(60, value(QL1S("idlenessTimeSecs"), 900).toInt(), 3599);
    int m = oldValue / 60;
    int s = oldValue - m * 60;
    return value(IDLENESS_AC_TIME, QTime(0, m, s)).toTime();
}

void PowerManagementSettings::setIdlenessACTime(QTime idlenessTime)
{
    setValue(IDLENESS_AC_TIME, idlenessTime);
}

int PowerManagementSettings::getIdlenessBatteryAction()
{
    // defaults to nothing (-1) and eventually load legacy value
    int oldValue = value(QL1S("idlenessAction"), -1).toInt();
    return value(IDLENESS_BATTERY_ACTION_KEY, oldValue).toInt();
}

void PowerManagementSettings::setIdlenessBatteryAction(int idlenessAction)
{
    setValue(IDLENESS_BATTERY_ACTION_KEY, idlenessAction);
}

QTime PowerManagementSettings::getIdlenessBatteryTime()
{
    // 1. The default value is 15 minutes (the legacy key is also read);
    // 2. Intervals greater than or equal to one hour aren't supported; and
    // 3. Intervals smaller than one minute are prevented.
    int oldValue = qBound(60, value(QL1S("idlenessTimeSecs"), 900).toInt(), 3599);
    int m = oldValue / 60;
    int s = oldValue - m * 60;
    return value(IDLENESS_BATTERY_TIME, QTime(0, m, s)).toTime();
}

void PowerManagementSettings::setIdlenessBatteryTime(QTime idlenessTime)
{
    setValue(IDLENESS_BATTERY_TIME, idlenessTime);
}

bool PowerManagementSettings::isIdlenessBacklightWatcherEnabled()
{
    return value(ENABLE_IDLENESS_BACKLIGHT_WATCHER_KEY, false).toBool();
}

void PowerManagementSettings::setIdlenessBacklightWatcherEnabled(bool idlenessBacklightWatcherEnabled)
{
    setValue(ENABLE_IDLENESS_BACKLIGHT_WATCHER_KEY, idlenessBacklightWatcherEnabled);
}

QTime PowerManagementSettings::getIdlenessBacklightTime()
{
    // default to 1 minute
    return value(IDLENESS_BACKLIGHT_TIME, QTime(0, 1)).toTime();
}

void PowerManagementSettings::setIdlenessBacklightTime(QTime idlenessBacklightTime)
{
    setValue(IDLENESS_BACKLIGHT_TIME, idlenessBacklightTime);
}

int PowerManagementSettings::getBacklight()
{
    return value(IDLENESS_BACKLIGHT, 50).toInt();
}

void PowerManagementSettings::setBacklight(int backlight)
{
    setValue(IDLENESS_BACKLIGHT, backlight);
}

bool PowerManagementSettings::isIdlenessBacklightOnBatteryDischargingEnabled()
{
    return value(IDLENESS_BACKLIGHT_ON_BATTERY_DISCHARGING, true).toBool();
}

void PowerManagementSettings::setIdlenessBacklightOnBatteryDischargingEnabled(bool enabled)
{
    setValue(IDLENESS_BACKLIGHT_ON_BATTERY_DISCHARGING, enabled);
}

bool PowerManagementSettings::isDisableIdlenessWhenFullscreenEnabled()
{
    return value(DISABLE_IDLENESS_WHEN_FULLSCREEN, false).toBool();
}

void PowerManagementSettings::setDisableIdlenessWhenFullscreen(bool enabled)
{
    setValue(DISABLE_IDLENESS_WHEN_FULLSCREEN, enabled);
}

int PowerManagementSettings::getPowerKeyAction()
{
    return value(POWER_KEY_ACTION, LXQt::Power::Action::PowerShutdown).toInt();
}

void PowerManagementSettings::setPowerKeyAction(int action)
{
    setValue(POWER_KEY_ACTION, action);
}

int PowerManagementSettings::getSuspendKeyAction()
{
    return value(SUSPEND_KEY_ACTION, LXQt::Power::Action::PowerSuspend).toInt();
}

void PowerManagementSettings::setSuspendKeyAction(int action)
{
    setValue(SUSPEND_KEY_ACTION, action);
}

int PowerManagementSettings::getHibernateKeyAction()
{
    return value(HIBERNATE_KEY_ACTION, LXQt::Power::Action::PowerHibernate).toInt();
}

void PowerManagementSettings::setHibernateKeyAction(int action)
{
    setValue(HIBERNATE_KEY_ACTION, action);
}

bool PowerManagementSettings::isIdlenessWatcherPaused() const
{
    return mIdlenessWatcherPaused;
}

void PowerManagementSettings::setIdlenessWatcherPaused(bool idlenessWatcherPaused) {
    mIdlenessWatcherPaused = idlenessWatcherPaused;
}

