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

BatteryWatcherSettings::BatteryWatcherSettings(LxQt::Settings *settings, QWidget *parent) :
    QWidget(parent),
    mUi(new Ui::BatteryWatcherSettings)

{
    mSettings = settings;
    mUi->setupUi(this);
   
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
    fillComboBox(mUi->actionComboBox);
    loadValueFromSettings(mSettings, mUi->actionComboBox, POWERLOWACTION_KEY, NOTHING);
    mUi->warningSpinBox->setValue(mSettings->value(POWERLOWWARNING_KEY, 30).toInt());
    mUi->levelSpinBox->setValue(mSettings->value(POWERLOWLEVEL_KEY, 15).toInt());
    mUi->useThemeIconsCheckBox->setChecked(mSettings->value(USETHEMEICONS_KEY).toBool());
}

void BatteryWatcherSettings::saveSettings()
{
    mSettings->setValue(POWERLOWACTION_KEY, mUi->actionComboBox->itemData(mUi->actionComboBox->currentIndex()).toInt());
    mSettings->setValue(POWERLOWWARNING_KEY, mUi->warningSpinBox->value());
    mSettings->setValue(POWERLOWLEVEL_KEY, mUi->levelSpinBox->value());
    mSettings->setValue(USETHEMEICONS_KEY, mUi->useThemeIconsCheckBox->isChecked());
}

