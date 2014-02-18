#include <QDebug>
#include <lxqt/LxQtPower>
#include "powermanagementsettings.h"

namespace PowerManagementSettingsConstants 
{
    const QString PERFORM_FIRST_RUN_CHECK = "performFirstRunCheck";
    const QString ENABLE_BATTERY_WATCHER_KEY = "enableBatteryWatcher";
    const QString ENABLE_LID_WATCHER_KEY = "enableLidWatcher";
    const QString ENABLE_IDLENESS_WATCHER_KEY = "enableIdlenessWatcher";
    const QString LID_CLOSED_ACTION_KEY = "lidClosedAction";
    const QString LID_CLOSED_AC_ACTION_KEY = "lidClosedAcAction";
    const QString LID_CLOSED_EXT_MON_ACTION_KEY = "lidClosedExtMonAction";
    const QString LID_CLOSED_EXT_MON_AC_ACTION_KEY = "lidClosedExtMonAcAction";
    const QString ENABLE_EXT_MON_LIDCLOSED_ACTIONS_KEY = "enableExtMonLidClosedActions";
    const QString POWER_LOW_ACTION_KEY = "powerLowAction";
    const QString POWER_LOW_WARNING_KEY = "powerLowWarning";
    const QString POWER_LOW_LEVEL_KEY = "powerLowLevel";
    const QString USE_THEME_ICONS_KEY = "useThemeIcons";
    const QString IDLENESS_ACTION_KEY = "idlenessAction";
    const QString IDLENESS_TIME_MINS_KEY = "idlenessTimeMins";
    const QString IDLENESS_TIME_SECS_KEY = "idlenessTimeSecs";
}

using namespace PowerManagementSettingsConstants;

PowerManagementSettings::PowerManagementSettings(QObject* parent) : LxQt::Settings("lxqt-powermanagement")
{
}

PowerManagementSettings::~PowerManagementSettings()
{
}

bool PowerManagementSettings::isPerformFirstRunCheck()
{
    return value(PERFORM_FIRST_RUN_CHECK, true).toBool();
}

void PowerManagementSettings::setPerformFirstRunCheck(bool performFirstRunCheck)
{
    setValue(PERFORM_FIRST_RUN_CHECK, performFirstRunCheck);
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
    return value(POWER_LOW_ACTION_KEY, 0).toInt();
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
    qDebug() << "PowerManagementSettings, setLidWatcherEnabled(" << lidWatcherEnabled << ")";
    setValue(ENABLE_LID_WATCHER_KEY, lidWatcherEnabled);
}

int PowerManagementSettings::getLidClosedAcAction()
{
    return value(LID_CLOSED_AC_ACTION_KEY, 0).toInt();
}

void PowerManagementSettings::setLidClosedAcAction(int lidClosedAcAction)
{
    setValue(LID_CLOSED_AC_ACTION_KEY, lidClosedAcAction);
}

int PowerManagementSettings::getLidClosedAction()
{
    return value(LID_CLOSED_ACTION_KEY, 0).toInt();
}

void PowerManagementSettings::setLidClosedAction(int lidClosedAction)
{
    setValue(LID_CLOSED_ACTION_KEY, lidClosedAction);
}

int PowerManagementSettings::getLidClosedExtMonAcAction()
{
    return value(LID_CLOSED_EXT_MON_AC_ACTION_KEY, 0).toInt();
}

void PowerManagementSettings::setLidClosedExtMonAcAction(int lidClosedExtMonAcAction)
{
    setValue(LID_CLOSED_EXT_MON_AC_ACTION_KEY, lidClosedExtMonAcAction);
}

int PowerManagementSettings::getLidClosedExtMonAction()
{
    return value(LID_CLOSED_EXT_MON_ACTION_KEY, 0).toInt();
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

int PowerManagementSettings::getIdlenessAction()
{
    return value(IDLENESS_ACTION_KEY, 0).toInt();
}

void PowerManagementSettings::setIdlenessAction(int idlenessAction)
{
    setValue(IDLENESS_ACTION_KEY, idlenessAction);
}

int PowerManagementSettings::getIdlenessTimeMins()
{
    return value(IDLENESS_TIME_MINS_KEY, 0).toInt();
}

void PowerManagementSettings::setIdlenessTimeMins(int idlenessTimeMins)
{
    setValue(IDLENESS_TIME_MINS_KEY, idlenessTimeMins);
}

int PowerManagementSettings::getIdlenessTimeSecs()
{
    return value(IDLENESS_TIME_SECS_KEY, 5).toInt();
}

void PowerManagementSettings::setIdlenessTimeSecs(int idlenessTimeSecs)
{
    setValue(IDLENESS_TIME_SECS_KEY, idlenessTimeSecs);
}


bool PowerManagementSettings::isIdlenessWatcherEnabled()
{
    return value(ENABLE_IDLENESS_WATCHER_KEY, false).toBool();
}

void PowerManagementSettings::setIdlenessWatcherEnabled(bool idlenessWatcherEnabled)
{
    setValue(ENABLE_IDLENESS_WATCHER_KEY, idlenessWatcherEnabled);
}


