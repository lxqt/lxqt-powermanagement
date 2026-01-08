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

#include <QComboBox>
#include <QLineEdit>
#include <QDebug>

#include "idlenesswatchersettings.h"
#include "ui_idlenesswatchersettings.h"
#include "helpers.h"

IdlenessWatcherSettings::IdlenessWatcherSettings(QWidget *parent) :
    QWidget(parent),
    mSettings(),
    mUi(new Ui::IdlenessWatcherSettings)
{
    mUi->setupUi(this);
    fillComboBox(mUi->idleACActionComboBox, false, false);
    fillComboBox(mUi->idleBatteryActionComboBox, false, false);

    // Set the minimum value of monitor spin-boxes to "noMonitorIdlenessWatcher".
    mUi->idleACMonitorOffTimeEdit->setMinimumTime(PowerManagementSettings::noMonitorIdlenessWatcher);
    mUi->idleBatteryMonitorOffTimeEdit->setMinimumTime(PowerManagementSettings::noMonitorIdlenessWatcher);

    // Set the minimum value of action spin-boxes to the same value for the sake of consistency.
    mUi->idleACTimeEdit->setMinimumTime(PowerManagementSettings::noMonitorIdlenessWatcher);
    mUi->idleBatteryTimeEdit->setMinimumTime(PowerManagementSettings::noMonitorIdlenessWatcher);

    // Also, set the maximum values of monitor spin-boxes to the action values properly.
    QTime ACMonitorMax = mUi->idleACMonitorOffTimeEdit->maximumTime();
    QTime BatteryMonitorMax = mUi->idleBatteryMonitorOffTimeEdit->maximumTime();
    if (currentValue(mUi->idleACActionComboBox) != -1) {
        mUi->idleACMonitorOffTimeEdit->setMaximumTime(mUi->idleACTimeEdit->time());
    }
    if (currentValue(mUi->idleBatteryActionComboBox) != -1) {
        mUi->idleBatteryMonitorOffTimeEdit->setMaximumTime(mUi->idleBatteryTimeEdit->time());
    }
    connect(mUi->idleACTimeEdit, &QTimeEdit::timeChanged, this, [this](QTime time) {
        if (currentValue(mUi->idleACActionComboBox) != -1) {
            mUi->idleACMonitorOffTimeEdit->setMaximumTime(time);
        }
    });
    connect(mUi->idleACActionComboBox, &QComboBox::currentIndexChanged, this, [this, ACMonitorMax](int) {
        if (currentValue(mUi->idleACActionComboBox) == -1) {
            mUi->idleACMonitorOffTimeEdit->setMaximumTime(ACMonitorMax);
        }
        else {
            mUi->idleACMonitorOffTimeEdit->setMaximumTime(mUi->idleACTimeEdit->time());
        }
    });
    connect(mUi->idleBatteryTimeEdit, &QTimeEdit::timeChanged, this, [this](QTime time) {
        if (currentValue(mUi->idleBatteryActionComboBox) != -1) {
            mUi->idleBatteryMonitorOffTimeEdit->setMaximumTime(time);
        }
    });
    connect(mUi->idleBatteryActionComboBox, &QComboBox::currentIndexChanged, this, [this, BatteryMonitorMax](int) {
        if (currentValue(mUi->idleBatteryActionComboBox) == -1) {
            mUi->idleBatteryMonitorOffTimeEdit->setMaximumTime(BatteryMonitorMax);
        }
        else {
            mUi->idleBatteryMonitorOffTimeEdit->setMaximumTime(mUi->idleBatteryTimeEdit->time());
        }
    });

    // Set tooltips.
    QString tooltip = tr("Minutes:Seconds (min: 01:30)");
    mUi->idleACTimeEdit->setToolTip(tooltip);
    mUi->idleBatteryTimeEdit->setToolTip(tooltip);
    mUi->idleACActionLabel->setToolTip(tooltip);
    mUi->idleBatteryActionLabel->setToolTip(tooltip);

    // Add a note about disabling monitor watchers.
    tooltip += QStringLiteral("\n") + tr("Set to minimum to disable");
    mUi->idleACMonitorOffTimeEdit->setToolTip(tooltip);
    mUi->idleBatteryMonitorOffTimeEdit->setToolTip(tooltip);
    mUi->idleACMonitorLabel->setToolTip(tooltip);
    mUi->idleBatteryMonitorLabel->setToolTip(tooltip);

    mBacklight = new LXQt::Backlight(this);
    // If if no backlight support then disable backlight control:
    if(! mBacklight->isBacklightAvailable()) {
        mUi->idlenessBacklightWatcherGroupBox->setEnabled(false);
        mUi->idlenessBacklightWatcherGroupBox->setChecked(false);
    } else {
        mBacklightActualValue = mBacklight->getBacklight();
    }

    // Handle the monitor command separately.
    if (QGuiApplication::platformName() == QStringLiteral("wayland")) {
        // Add some known commands that work fine.
        mUi->monitorComboBox->addItem(QStringLiteral("Hyprland"), QStringLiteral("hyprctl dispatch dpms off"));
        mUi->monitorComboBox->addItem(QStringLiteral("KWin-Wayland"), QStringLiteral("kscreen-doctor --dpms off"));
        mUi->monitorComboBox->addItem(QStringLiteral("Niri"), QStringLiteral("niri msg action power-off-monitors"));
    }
    else {
        mUi->monitorGroupBox->hide();
    }

    mFirstLoad = true; // not a resetting
    loadSettings();

    connect(mUi->idlenessWatcherGroupBox, &QGroupBox::toggled, this, &IdlenessWatcherSettings::settingsChanged);
    connect(mUi->idleACActionComboBox, &QComboBox::currentIndexChanged, this, &IdlenessWatcherSettings::settingsChanged);
    connect(mUi->idleACTimeEdit, &QTimeEdit::dateTimeChanged, this, &IdlenessWatcherSettings::settingsChanged);
    connect(mUi->idleACMonitorOffTimeEdit, &QTimeEdit::dateTimeChanged, this, &IdlenessWatcherSettings::settingsChanged);
    connect(mUi->idleBatteryActionComboBox, &QComboBox::currentIndexChanged, this, &IdlenessWatcherSettings::settingsChanged);
    connect(mUi->idleBatteryTimeEdit, &QTimeEdit::dateTimeChanged, this, &IdlenessWatcherSettings::settingsChanged);
    connect(mUi->idleBatteryMonitorOffTimeEdit, &QTimeEdit::dateTimeChanged, this, &IdlenessWatcherSettings::settingsChanged);

    connect(mUi->checkBacklightButton, &QPushButton::pressed, this, &IdlenessWatcherSettings::backlightCheckButtonPressed);
    connect(mUi->checkBacklightButton, &QPushButton::released, this, &IdlenessWatcherSettings::backlightCheckButtonReleased);

    connect(mUi->idlenessBacklightWatcherGroupBox, &QGroupBox::toggled, this, &IdlenessWatcherSettings::settingsChanged);
    connect(mUi->backlightSlider, &QSlider::valueChanged, this, &IdlenessWatcherSettings::settingsChanged);
    connect(mUi->idleTimeBacklightTimeEdit, &QTimeEdit::dateTimeChanged, this, &IdlenessWatcherSettings::settingsChanged);
    connect(mUi->onBatteryDischargingCheckBox, &QCheckBox::toggled, this, &IdlenessWatcherSettings::settingsChanged);

    connect(mUi->disableIdlenessFullscreenBox, &QCheckBox::toggled, this, &IdlenessWatcherSettings::settingsChanged);

    if (QGuiApplication::platformName() == QStringLiteral("wayland")) {
        connect(mUi->monitorComboBox, &QComboBox::activated, this, [this](int index) {
            if (index > -1) {
                // don't use setText(), because it resets modification
                mUi->monitorLineEdit->selectAll();
                mUi->monitorLineEdit->insert(mUi->monitorComboBox->itemData(index).toString());
            }
        });
        connect(mUi->monitorLineEdit, &QLineEdit::textChanged, this, &IdlenessWatcherSettings::settingsChanged);
    }
}

IdlenessWatcherSettings::~IdlenessWatcherSettings()
{
    delete mUi;
}

void IdlenessWatcherSettings::loadSettings()
{
    mUi->idlenessWatcherGroupBox->setChecked(mSettings.isIdlenessWatcherEnabled());

    setComboBoxToValue(mUi->idleACActionComboBox, mSettings.getIdlenessACAction());
    mUi->idleACTimeEdit->setTime(mSettings.getIdlenessACTime());

    setComboBoxToValue(mUi->idleBatteryActionComboBox, mSettings.getIdlenessBatteryAction());
    mUi->idleBatteryTimeEdit->setTime(mSettings.getIdlenessBatteryTime());

    // monitor settings
    mUi->idleACMonitorOffTimeEdit->setTime(mSettings.getMonitorACIdleTime());
    mUi->idleBatteryMonitorOffTimeEdit->setTime(mSettings.getMonitorBatteryIdleTime());

    if(mBacklight->isBacklightAvailable()) {
        mUi->idlenessBacklightWatcherGroupBox->setChecked(mSettings.isIdlenessBacklightWatcherEnabled());
        mUi->idleTimeBacklightTimeEdit->setTime(mSettings.getIdlenessBacklightTime());
        mUi->backlightSlider->setValue(mSettings.getBacklight());
        mUi->onBatteryDischargingCheckBox->setChecked(mSettings.isIdlenessBacklightOnBatteryDischargingEnabled());
    }
    mUi->disableIdlenessFullscreenBox->setChecked(mSettings.isDisableIdlenessWhenFullscreenEnabled());

    if (QGuiApplication::platformName() == QStringLiteral("wayland")) {
        LXQt::Settings powerSetting(QStringLiteral("power"));
        if (mFirstLoad) {
            mMonitorOffCommand = powerSetting.value(QStringLiteral("monitorOffCommand_wayland"));
        }
        else {
            // needed with resetting
            powerSetting.setValue(QStringLiteral("monitorOffCommand_wayland"), mMonitorOffCommand);
        }
        mUi->monitorComboBox->setCurrentIndex(mUi->monitorComboBox->findData(mMonitorOffCommand));
        mUi->monitorLineEdit->setText(mMonitorOffCommand.toString());
    }
    mFirstLoad = false;
}

void IdlenessWatcherSettings::saveSettings()
{
    mSettings.setIdlenessWatcherEnabled(mUi->idlenessWatcherGroupBox->isChecked());

    mSettings.setIdlenessACAction(currentValue(mUi->idleACActionComboBox));
    mSettings.setIdlenessACTime(mUi->idleACTimeEdit->time());

    mSettings.setIdlenessBatteryAction(currentValue(mUi->idleBatteryActionComboBox));
    mSettings.setIdlenessBatteryTime(mUi->idleBatteryTimeEdit->time());

    // monitor settings
    mSettings.setMonitorACIdleTime(mUi->idleACMonitorOffTimeEdit->time());
    mSettings.setMonitorBatteryIdleTime(mUi->idleBatteryMonitorOffTimeEdit->time());

    mSettings.setIdlenessBacklightWatcherEnabled(mBacklight->isBacklightAvailable() ? mUi->idlenessBacklightWatcherGroupBox->isChecked() : false);
    mSettings.setIdlenessBacklightTime(mUi->idleTimeBacklightTimeEdit->time());
    mSettings.setBacklight(mUi->backlightSlider->value());
    mSettings.setIdlenessBacklightOnBatteryDischargingEnabled(mUi->onBatteryDischargingCheckBox->isChecked());
    mSettings.setDisableIdlenessWhenFullscreen(mUi->disableIdlenessFullscreenBox->isChecked());

    if (QGuiApplication::platformName() == QStringLiteral("wayland")) {
        LXQt::Settings powerSetting(QStringLiteral("power"));
        powerSetting.setValue(QStringLiteral("monitorOffCommand_wayland"), mUi->monitorLineEdit->text());
        mUi->monitorComboBox->setCurrentIndex(mUi->monitorComboBox->findData(mUi->monitorLineEdit->text()));
    }
}

void IdlenessWatcherSettings::backlightCheckButtonPressed()
{
    if(mBacklight->isBacklightAvailable()) {
        mBacklightActualValue = mBacklight->getBacklight();
        mBacklight->setBacklight((float)mBacklightActualValue * (float)(mUi->backlightSlider->value())/100.0f);
    }
}

void IdlenessWatcherSettings::backlightCheckButtonReleased()
{
    if(mBacklight->isBacklightAvailable()) {
        mBacklight->setBacklight(mBacklightActualValue);
    }
}
