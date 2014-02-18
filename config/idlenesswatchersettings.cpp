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

#include "idlenesswatchersettings.h"
#include "ui_idlenesswatchersettings.h"
#include "helpers.h"

IdlenessWatcherSettings::IdlenessWatcherSettings(QWidget *parent) :
    QWidget(parent), 
    mSettings(), 
    mUi(new Ui::IdlenessWatcherSettings)
{
    mUi->setupUi(this);
    fillComboBox(mUi->idleActionComboBox);
    connect(mUi->idlenessWatcherGroupBox, SIGNAL(clicked()), SLOT(saveSettings()));
    connect(mUi->idleActionComboBox, SIGNAL(activated(int)), SLOT(saveSettings()));
    connect(mUi->idleTimeMinutesSpinBox, SIGNAL(valueChanged(int)), SLOT(saveSettings()));
    connect(mUi->idleTimeSecondsSpinBox, SIGNAL(valueChanged(int)), SLOT(saveSettings()));
}

IdlenessWatcherSettings::~IdlenessWatcherSettings()
{
    delete mUi;
}

void IdlenessWatcherSettings::loadSettings()
{
    qDebug() << "IdlenessWatcherSettings:loadSettings";

    mUi->idlenessWatcherGroupBox->setChecked(mSettings.isIdlenessWatcherEnabled());
    setComboBoxToValue(mUi->idleActionComboBox, mSettings.getIdlenessAction());
    mUi->idleTimeMinutesSpinBox->setValue(mSettings.getIdlenessTimeMins());
    mUi->idleTimeSecondsSpinBox->setValue(mSettings.getIdlenessTimeSecs());
}

void IdlenessWatcherSettings::saveSettings()
{
    qDebug() << "IdlenessWatcherSettings:saveSettings";

    mSettings.setIdlenessWatcherEnabled(mUi->idlenessWatcherGroupBox->isChecked());
    mSettings.setIdlenessAction(currentValue(mUi->idleActionComboBox));
    mSettings.setIdlenessTimeMins(mUi->idleTimeMinutesSpinBox->value());
    mSettings.setIdlenessTimeSecs(mUi->idleTimeSecondsSpinBox->value());
}
