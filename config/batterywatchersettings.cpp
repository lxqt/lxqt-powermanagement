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
#include <LXQt/Settings>

#include <QDebug>
#include <QLabel>

#include "batterywatchersettings.h"
#include "ui_batterywatchersettings.h"
#include "powermanagementsettings.h"

BatteryWatcherSettings::BatteryWatcherSettings(QWidget *parent) :
    QWidget(parent),
    mSettings(),
    mUi(new Ui::BatteryWatcherSettings),
    mIconProducer()

{
    mUi->setupUi(this);
    fillComboBox(mUi->actionComboBox);
    mUi->chargeLevelSlider->setValue(53);

    connect(mUi->groupBox, SIGNAL(clicked()), this, SLOT(saveSettings()));
    connect(mUi->actionComboBox, SIGNAL(activated(int)), this, SLOT(saveSettings()));
    connect(mUi->warningSpinBox, SIGNAL(editingFinished()), this, SLOT(saveSettings()));
    connect(mUi->levelSpinBox, SIGNAL(editingFinished()), this, SLOT(saveSettings()));
    connect(mUi->showIconCheckBox, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
    connect(mUi->showIconCheckBox, SIGNAL(clicked(bool)), mUi->previewBox, SLOT(setEnabled(bool)));
    connect(mUi->useThemeIconsCheckBox, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
    connect(mUi->useThemeIconsCheckBox, SIGNAL(clicked(bool)), this, SLOT(updatePreview()));
    connect(mUi->chargeLevelSlider, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
    connect(LxQt::Settings::globalSettings(), SIGNAL(iconThemeChanged()), this, SLOT(updatePreview()));
    updatePreview();
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
    mUi->showIconCheckBox->setChecked(mSettings.isShowIcon());
    mUi->useThemeIconsCheckBox->setChecked(mSettings.isUseThemeIcons());
}

void BatteryWatcherSettings::saveSettings()
{
    mSettings.setBatteryWatcherEnabled(mUi->groupBox->isChecked());
    mSettings.setPowerLowAction(currentValue(mUi->actionComboBox));
    mSettings.setPowerLowWarningTime(mUi->warningSpinBox->value());
    mSettings.setPowerLowLevel(mUi->levelSpinBox->value());
    mSettings.setShowIcon(mUi->showIconCheckBox->isChecked());
    mSettings.setUseThemeIcons(mUi->useThemeIconsCheckBox->isChecked());
}

void BatteryWatcherSettings::updatePreview()
{
    float chargeLevel = mUi->chargeLevelSlider->value();
    mUi->chargingIcon->setPixmap(mIconProducer.icon(chargeLevel, false).pixmap(QSize(48, 48)));
    mUi->chargingLabel->setText(mIconProducer.iconName(chargeLevel, false));
    mUi->dischargingIcon->setPixmap(mIconProducer.icon(chargeLevel, true).pixmap(QSize(48, 48)));
    mUi->dischargingLabel->setText(mIconProducer.iconName(chargeLevel, true));
    mUi->chargeLevelLabel->setText(tr("Level: %1%").arg(chargeLevel));
}
