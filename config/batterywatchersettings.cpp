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
#include "common.h"

BatteryWatcherSettings::BatteryWatcherSettings(QWidget *parent) :
    QWidget(parent),
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
    PowerManagementSettings settings;

    mUi->groupBox->setChecked(settings.isBatteryWatcherEnabled());
    
    setComboBoxToValue(mUi->actionComboBox, settings.getPowerLowAction());
    mUi->warningSpinBox->setValue(settings.getPowerLowWarningTime());
    mUi->levelSpinBox->setValue(settings.getPowerLowLevel());
    mUi->useThemeIconsCheckBox->setChecked(settings.isUseThemeIcons());
}

void BatteryWatcherSettings::saveSettings()
{
    PowerManagementSettings settings;
    
    settings.setBatteryWatcherEnabled(mUi->groupBox->isChecked());
    
    settings.setPowerLowAction(currentValue(mUi->actionComboBox));
    settings.setPowerLowWarningTime(mUi->warningSpinBox->value());
    settings.setPowerLowLevel(mUi->levelSpinBox->value());
    settings.setUseThemeIcons(mUi->useThemeIconsCheckBox->isChecked());
}

