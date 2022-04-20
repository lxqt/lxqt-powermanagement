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
#include <LXQt/Settings>

#include <QDebug>
#include <QLabel>
#include <QGroupBox>
#include <Solid/Battery>

#include "batterywatchersettings.h"
#include "ui_batterywatchersettings.h"
#include "powermanagementsettings.h"

static inline void fillIconTypeCombo(QComboBox* comboBox)
{
    comboBox->clear();
    comboBox->addItem(BatteryWatcherSettings::tr("from theme", "icons"), PowerManagementSettings::ICON_THEME);
    comboBox->addItem(BatteryWatcherSettings::tr("Circle", "icons"), PowerManagementSettings::ICON_CIRCLE);
    comboBox->addItem(BatteryWatcherSettings::tr("Circle with percentage", "icons"), PowerManagementSettings::ICON_CIRCLE_ENHANCED);
    comboBox->addItem(BatteryWatcherSettings::tr("Battery with percentage and background", "icons"), PowerManagementSettings::ICON_BATTERY);
    comboBox->addItem(BatteryWatcherSettings::tr("Battery with percentage", "icons"), PowerManagementSettings::ICON_BATTERY_OPAQUE);
}

BatteryWatcherSettings::BatteryWatcherSettings(QWidget *parent) :
    QWidget(parent),
    mSettings(),
    mUi(new Ui::BatteryWatcherSettings),
    mChargingIconProducer(),
    mDischargingIconProducer()

{
    mUi->setupUi(this);
    fillComboBox(mUi->actionComboBox);
    fillIconTypeCombo(mUi->iconTypeComboBox);
    mUi->chargeLevelSlider->setValue(53);
    mChargingIconProducer.updateState(Solid::Battery::Charging);
    mDischargingIconProducer.updateState(Solid::Battery::Discharging);

    connect(mUi->groupBox, &QGroupBox::clicked, this, &BatteryWatcherSettings::saveSettings);
    connect(mUi->actionComboBox, QOverload<int>::of(&QComboBox::activated), this, &BatteryWatcherSettings::saveSettings);
    connect(mUi->warningSpinBox, &QSpinBox::editingFinished, this, &BatteryWatcherSettings::saveSettings);
    connect(mUi->levelSpinBox, &QSpinBox::editingFinished, this, &BatteryWatcherSettings::saveSettings);
    connect(mUi->showIconCheckBox, &QCheckBox::clicked, this, &BatteryWatcherSettings::saveSettings);
    connect(mUi->showIconCheckBox, &QCheckBox::clicked, mUi->previewBox, &QGroupBox::setEnabled);
    connect(mUi->iconTypeComboBox, QOverload<int>::of(&QComboBox::activated), this, &BatteryWatcherSettings::saveSettings);
    connect(mUi->iconTypeComboBox, QOverload<int>::of(&QComboBox::activated), this, &BatteryWatcherSettings::updatePreview);
    connect(mUi->chargeLevelSlider, &QSlider::valueChanged, this, &BatteryWatcherSettings::updatePreview);
    connect(&mChargingIconProducer, &IconProducer::iconChanged, this, &BatteryWatcherSettings::onChargeIconChanged);
    connect(&mDischargingIconProducer, &IconProducer::iconChanged, this, &BatteryWatcherSettings::onDischargeIconChanged);
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
    setComboBoxToValue(mUi->iconTypeComboBox, mSettings.getIconType());
}

void BatteryWatcherSettings::saveSettings()
{
    mSettings.setBatteryWatcherEnabled(mUi->groupBox->isChecked());
    mSettings.setPowerLowAction(currentValue(mUi->actionComboBox));
    mSettings.setPowerLowWarningTime(mUi->warningSpinBox->value());
    mSettings.setPowerLowLevel(mUi->levelSpinBox->value());
    mSettings.setShowIcon(mUi->showIconCheckBox->isChecked());
    mSettings.setIconType(static_cast<PowerManagementSettings::IconType>(currentValue(mUi->iconTypeComboBox)));
}

void BatteryWatcherSettings::updatePreview()
{
    mUi->previewBox->setTitle(tr("Preview") +  QString::fromLatin1(" (%1)").arg(mSettings.isUseThemeIcons() ? QIcon::themeName() : tr("built in")));

    int chargePercent = mUi->chargeLevelSlider->value();
    mChargingIconProducer.updateChargePercent(chargePercent);
    mDischargingIconProducer.updateChargePercent(chargePercent);
    mUi->chargeLevelLabel->setText(tr("Level: %1%").arg(chargePercent));
}

void BatteryWatcherSettings::onChargeIconChanged()
{
    mUi->chargingIcon->setPixmap(mChargingIconProducer.mIcon.pixmap(mUi->chargingIcon->size()));
    mUi->chargingLabel->setText(mChargingIconProducer.mIconName);
}

void BatteryWatcherSettings::onDischargeIconChanged()
{
    mUi->dischargingIcon->setPixmap(mDischargingIconProducer.mIcon.pixmap(mUi->dischargingIcon->size()));
    mUi->dischargingLabel->setText(mDischargingIconProducer.mIconName);
}
