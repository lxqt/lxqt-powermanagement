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
#include <QComboBox>
#include <QDebug>

#include "lidwatchersettings.h"
#include "ui_lidwatchersettings.h"
#include "common.h"

LidWatcherSettings::LidWatcherSettings(QWidget *parent) :
    QWidget(parent),
    mUi(new Ui::LidWatcherSettings)
{
    mUi->setupUi(this);
    
    fillComboBox(mUi->onBatteryActionComboBox);
    fillComboBox(mUi->onAcActionComboBox);
    fillComboBox(mUi->extMonOnBatteryActionComboBox);
    fillComboBox(mUi->extMonOnAcActionComboBox);
    
    connect(mUi->lidWatcherSettingsGroupBox, SIGNAL(clicked()), this, SLOT(saveSettings()));
    connect(mUi->onBatteryActionComboBox, SIGNAL(activated(int)), this, SLOT(saveSettings()));
    connect(mUi->onAcActionComboBox, SIGNAL(activated(int)), this, SLOT(saveSettings()));
    connect(mUi->extMonOnBatteryActionComboBox, SIGNAL(activated(int)), this, SLOT(saveSettings()));
    connect(mUi->extMonOnAcActionComboBox, SIGNAL(activated(int)), this, SLOT(saveSettings()));
    connect(mUi->extMonGroupBox, SIGNAL(clicked()), this, SLOT(saveSettings()));
}

LidWatcherSettings::~LidWatcherSettings()
{
    delete mUi;
}

void LidWatcherSettings::loadSettings()
{
    qDebug() << "LidWatcher::loadSettings...";
    PowerManagementSettings settings;
    
    mUi->lidWatcherSettingsGroupBox->setChecked(settings.isLidWatcherEnabled());
    
    setComboBoxToValue(mUi->onBatteryActionComboBox, settings.getLidClosedAction());
    setComboBoxToValue(mUi->onAcActionComboBox, settings.getLidClosedAcAction());
    
    mUi->extMonGroupBox->setChecked(settings.isEnableExtMonLidClosedActions());
    setComboBoxToValue(mUi->extMonOnBatteryActionComboBox, settings.getLidClosedExtMonAction());
    setComboBoxToValue(mUi->extMonOnAcActionComboBox, settings.getLidClosedExtMonAcAction());

}

void LidWatcherSettings::saveSettings()
{
    qDebug() << "LidWatcher::saveSettings...";
    
    PowerManagementSettings settings;
   
    settings.setLidWatcherEnabled(mUi->lidWatcherSettingsGroupBox->isChecked());
    
    settings.setLidClosedAction(currentValue(mUi->onBatteryActionComboBox));
    settings.setLidClosedAcAction(currentValue(mUi->onAcActionComboBox));
    
    settings.setEnableExtMonLidClosedActions(mUi->extMonGroupBox->isChecked());
    settings.setLidClosedExtMonAction(currentValue(mUi->extMonOnBatteryActionComboBox));
    settings.setLidClosedExtMonAcAction(currentValue(mUi->extMonOnAcActionComboBox));
}
