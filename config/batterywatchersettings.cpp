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
#include "batterywatchersettings.h"
#include "ui_batterywatchersettings.h"
#include "powermanagementsettings.h"

BatteryWatcherSettings::BatteryWatcherSettings(QWidget *parent) :
    QWidget(parent),
    mSettings(),
    mUi(new Ui::BatteryWatcherSettings)

{
    mUi->setupUi(this);
    fillComboBox(mUi->actionComboBox);
   
    connect(mUi->groupBox, SIGNAL(clicked()), this, SLOT(saveSettings()));
    connect(mUi->actionComboBox, SIGNAL(activated(int)), this, SLOT(saveSettings()));
    connect(mUi->warningSpinBox, SIGNAL(editingFinished()), this, SLOT(saveSettings()));
    connect(mUi->levelSpinBox, SIGNAL(editingFinished()), this, SLOT(saveSettings()));
    connect(mUi->useThemeIconsCheckBox, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
}

BatteryWatcherSettings::~BatteryWatcherSettings()
{
    delete mUi;
}

void BatteryWatcherSettings::loadSettings()
{

    mUi->groupBox->setChecked(mSettings.isBatteryWatcherEnabled());
    setComboBoxToValue(mUi->actionComboBox, mSettings.getPowerLowAction());
    mUi->warningSpinBox->setValue(mSettings.getPowerLowWarningTime());
    mUi->levelSpinBox->setValue(mSettings.getPowerLowLevel());
    mUi->useThemeIconsCheckBox->setChecked(mSettings.isUseThemeIcons());
}

void BatteryWatcherSettings::saveSettings()
{
    
    mSettings.setBatteryWatcherEnabled(mUi->groupBox->isChecked());
    mSettings.setPowerLowAction(currentValue(mUi->actionComboBox));
    mSettings.setPowerLowWarningTime(mUi->warningSpinBox->value());
    mSettings.setPowerLowLevel(mUi->levelSpinBox->value());
    mSettings.setUseThemeIcons(mUi->useThemeIconsCheckBox->isChecked());
}

