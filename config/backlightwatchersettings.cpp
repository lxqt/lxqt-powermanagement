/*
* Copyright (c) 2015 Dmitry Kontsevoy
*
* This file is part of the LXQt project. <http://lxqt.org>
* It is distributed under the LGPL 2.1 or later license.
* Please refer to the LICENSE file for a copy of the license, and
* the AUTHORS file for copyright and authorship information.
*/

#include "backlightwatchersettings.h"
#include "ui_backlightwatchersettings.h"

BacklightWatcherSettings::BacklightWatcherSettings(QWidget *parent):
    QWidget(parent),
    mSettings(),
    mUi(new Ui::BacklightWatcherSettings)
{
    mUi->setupUi(this);

    loadSettings();

    connect(mUi->groupBox, SIGNAL(clicked()), this, SLOT(saveSettings()));
    connect(mUi->backlightACSlider, SIGNAL(valueChanged(int)), this, SLOT(saveSettings()));
    connect(mUi->backlightBatterySlider, SIGNAL(valueChanged(int)), this, SLOT(saveSettings()));
}

BacklightWatcherSettings::~BacklightWatcherSettings()
{
}

void BacklightWatcherSettings::loadSettings()
{
    mUi->groupBox->setChecked(mSettings.isBacklightWatcherEnabled());
    mUi->backlightACSlider->setValue(mSettings.getBacklightACLevel());
    mUi->backlightBatterySlider->setValue(mSettings.getBacklightBatteryLevel());
}

void BacklightWatcherSettings::saveSettings()
{
    mSettings.setBacklightWatcherEnabled(mUi->groupBox->isChecked());
    mSettings.setBacklightACLevel(mUi->backlightACSlider->value());
    mSettings.setBacklightBatteryLevel(mUi->backlightBatterySlider->value());
}

