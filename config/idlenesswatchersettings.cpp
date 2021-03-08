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
#include <QDebug>

#include "idlenesswatchersettings.h"
#include "ui_idlenesswatchersettings.h"
#include "helpers.h"

#define MINIMUM_SECONDS 30

IdlenessWatcherSettings::IdlenessWatcherSettings(QWidget *parent) :
    QWidget(parent),
    mSettings(),
    mUi(new Ui::IdlenessWatcherSettings)
{
    mUi->setupUi(this);
    fillComboBox(mUi->idleACActionComboBox);
    fillComboBox(mUi->idleBatteryActionComboBox);

    mUi->idleACTimeEdit->setMinimumTime( QTime(0,0,MINIMUM_SECONDS) );
    mUi->idleBatteryTimeEdit->setMinimumTime( QTime(0,0,MINIMUM_SECONDS) );

    mBacklight = new LXQt::Backlight(this);
    // If if no backlight support then disable backlight control:
    if(! mBacklight->isBacklightAvailable()) {
        mUi->idlenessBacklightWatcherGroupBox->setEnabled(false);
        mUi->idlenessBacklightWatcherGroupBox->setChecked(false);
    } else {
        mBacklightActualValue = mBacklight->getBacklight();
    }

    mConnectSignals();
}


void IdlenessWatcherSettings::mConnectSignals()
{
    connect(mUi->idlenessWatcherGroupBox,          &QGroupBox::clicked,                         this, &IdlenessWatcherSettings::saveSettings);
    connect(mUi->idleACActionComboBox,             QOverload<int>::of(&QComboBox::activated),   this, &IdlenessWatcherSettings::saveSettings);
    connect(mUi->idleACTimeEdit,                   &QTimeEdit::editingFinished,                 this, &IdlenessWatcherSettings::saveSettings);
    connect(mUi->idleBatteryActionComboBox,        QOverload<int>::of(&QComboBox::activated),   this, &IdlenessWatcherSettings::saveSettings);
    connect(mUi->idleBatteryTimeEdit,              &QTimeEdit::editingFinished,                 this, &IdlenessWatcherSettings::saveSettings);

    connect(mUi->checkBacklightButton,             &QPushButton::pressed,                       this, &IdlenessWatcherSettings::backlightCheckButtonPressed);
    connect(mUi->checkBacklightButton,             &QPushButton::released,                      this, &IdlenessWatcherSettings::backlightCheckButtonReleased);

    connect(mUi->idlenessBacklightWatcherGroupBox, &QGroupBox::clicked,                         this, &IdlenessWatcherSettings::saveSettings);
    connect(mUi->backlightSlider,                  &QSlider::valueChanged,                      this, &IdlenessWatcherSettings::saveSettings);
    connect(mUi->idleTimeBacklightTimeEdit,        &QTimeEdit::editingFinished,                 this, &IdlenessWatcherSettings::saveSettings);
    connect(mUi->onBatteryDischargingCheckBox,     &QCheckBox::toggled,                         this, &IdlenessWatcherSettings::saveSettings);

    connect(mUi->disableIdlenessFullscreenBox,     &QCheckBox::toggled,                         this, &IdlenessWatcherSettings::saveSettings);
}

void IdlenessWatcherSettings::mDisconnectSignals()
{
    disconnect(mUi->idlenessWatcherGroupBox,          &QGroupBox::clicked,                         this, &IdlenessWatcherSettings::saveSettings);
    disconnect(mUi->idleACActionComboBox,             QOverload<int>::of(&QComboBox::activated),   this, &IdlenessWatcherSettings::saveSettings);
    disconnect(mUi->idleACTimeEdit,                   &QTimeEdit::editingFinished,                 this, &IdlenessWatcherSettings::saveSettings);
    disconnect(mUi->idleBatteryActionComboBox,        QOverload<int>::of(&QComboBox::activated),   this, &IdlenessWatcherSettings::saveSettings);
    disconnect(mUi->idleBatteryTimeEdit,              &QTimeEdit::editingFinished,                 this, &IdlenessWatcherSettings::saveSettings);

    disconnect(mUi->checkBacklightButton,             &QPushButton::pressed,                       this, &IdlenessWatcherSettings::backlightCheckButtonPressed);
    disconnect(mUi->checkBacklightButton,             &QPushButton::released,                      this, &IdlenessWatcherSettings::backlightCheckButtonReleased);

    disconnect(mUi->idlenessBacklightWatcherGroupBox, &QGroupBox::clicked,                         this, &IdlenessWatcherSettings::saveSettings);
    disconnect(mUi->backlightSlider,                  &QSlider::valueChanged,                      this, &IdlenessWatcherSettings::saveSettings);
    disconnect(mUi->idleTimeBacklightTimeEdit,        &QTimeEdit::editingFinished,                 this, &IdlenessWatcherSettings::saveSettings);
    disconnect(mUi->onBatteryDischargingCheckBox,     &QCheckBox::toggled,                         this, &IdlenessWatcherSettings::saveSettings);

    disconnect(mUi->disableIdlenessFullscreenBox,     &QCheckBox::toggled,                         this, &IdlenessWatcherSettings::saveSettings);
}

IdlenessWatcherSettings::~IdlenessWatcherSettings()
{
    delete mUi;
}

void IdlenessWatcherSettings::loadSettings()
{
    mDisconnectSignals();
    mUi->idlenessWatcherGroupBox->setChecked(mSettings.isIdlenessWatcherEnabled());

    setComboBoxToValue(mUi->idleACActionComboBox, mSettings.getIdlenessACAction());
    mUi->idleACTimeEdit->setTime(mSettings.getIdlenessACTime());

    setComboBoxToValue(mUi->idleBatteryActionComboBox, mSettings.getIdlenessBatteryAction());
    mUi->idleBatteryTimeEdit->setTime(mSettings.getIdlenessBatteryTime());

    if(mBacklight->isBacklightAvailable()) {
        mUi->idlenessBacklightWatcherGroupBox->setChecked(mSettings.isIdlenessBacklightWatcherEnabled());
        mUi->idleTimeBacklightTimeEdit->setTime(mSettings.getIdlenessBacklightTime());
        mUi->backlightSlider->setValue(mSettings.getBacklight());
        mUi->onBatteryDischargingCheckBox->setChecked(mSettings.isIdlenessBacklightOnBatteryDischargingEnabled());
    }
    mUi->disableIdlenessFullscreenBox->setChecked(mSettings.isDisableIdlenessWhenFullscreenEnabled());
    mConnectSignals();
}

void IdlenessWatcherSettings::saveSettings()
{
    mSettings.setIdlenessWatcherEnabled(mUi->idlenessWatcherGroupBox->isChecked());

    mSettings.setIdlenessACAction(currentValue(mUi->idleACActionComboBox));
    mSettings.setIdlenessACTime(mUi->idleACTimeEdit->time());

    mSettings.setIdlenessBatteryAction(currentValue(mUi->idleBatteryActionComboBox));
    mSettings.setIdlenessBatteryTime(mUi->idleBatteryTimeEdit->time());

    mSettings.setIdlenessBacklightWatcherEnabled(mBacklight->isBacklightAvailable() ? mUi->idlenessBacklightWatcherGroupBox->isChecked() : false);
    mSettings.setIdlenessBacklightTime(mUi->idleTimeBacklightTimeEdit->time());
    mSettings.setBacklight(mUi->backlightSlider->value());
    mSettings.setIdlenessBacklightOnBatteryDischargingEnabled(mUi->onBatteryDischargingCheckBox->isChecked());
    mSettings.setDisableIdlenessWhenFullscreen(mUi->disableIdlenessFullscreenBox->isChecked());
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
