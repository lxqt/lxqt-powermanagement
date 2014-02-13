#include <QDebug>
#include "common.h"

#define ENABLE_BATTERY_WATCHER_KEY "enableBatteryWatcher"
#define ENABLE_LID_WATCHER_KEY "enableLidWatcher"
#define ENABLE_IDLENESS_WATCHER_KEY "enableIdlenessWatcher"
#define LID_CLOSED_ACTION_KEY "lidClosedAction"
#define LID_CLOSED_AC_ACTION_KEY "lidClosedAcAction"
#define LID_CLOSED_EXT_MON_ACTION_KEY "lidClosedExtMonAction"
#define LID_CLOSED_EXT_MON_AC_ACTION_KEY "lidClosedExtMonAcAction"
#define ENABLE_EXT_MON_LIDCLOSED_ACTIONS_KEY "enableExtMonLidClosedActions"
#define POWER_LOW_ACTION_KEY "powerLowAction"
#define POWER_LOW_WARNING_KEY "powerLowWarning"
#define POWER_LOW_LEVEL_KEY "powerLowLevel"
#define USE_THEME_ICONS_KEY "useThemeIcons"
#define IDLENESS_ACTION_KEY "idlenessAction"
#define IDLENESS_TIME_MINS_KEY "idlenessTimeMins"
#define IDLENESS_TIME_SECS_KEY "idlenessTimeSecs"

PowerManagementSettings::PowerManagementSettings(QObject* parent) : LxQt::Settings("lxqt-autosuspend")
{
}

PowerManagementSettings::~PowerManagementSettings()
{
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


void fillComboBox(QComboBox* comboBox)
{
    comboBox->clear();
    comboBox->addItem(QObject::tr("Nothing"), NOTHING);
    comboBox->addItem(QObject::tr("Lock screen"), LOCK);
    comboBox->addItem(QObject::tr("Sleep"), SLEEP);
    comboBox->addItem(QObject::tr("Hibernate"), HIBERNATE);
    comboBox->addItem(QObject::tr("Shutdown"), POWEROFF);
}

void setComboBoxToValue(QComboBox* comboBox, int value)
{
    for (int index = 0; index < comboBox->count(); index++)
    {
        if (value == comboBox->itemData(index).toInt())
        {
            comboBox->setCurrentIndex(index);
            return;
        }
    }

    comboBox->setCurrentIndex(0);
}

int currentValue(QComboBox *comboBox)
{
    return comboBox->itemData(comboBox->currentIndex()).toInt();
}

