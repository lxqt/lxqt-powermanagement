/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXQt - a lightweight, Qt based, desktop toolset
 * http://lxqt.org
 *
 * Copyright: 2016 LXQt team
 * Authors:
 *   Paulo Lieuthier <paulolieuthier@gmail.com>
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

#include "lockscreensettings.h"
#include "ui_lockscreensettings.h"

LockScreenSettings::LockScreenSettings(QWidget *parent) :
    QWidget(parent),
    mSettings(),
    mUi(new Ui::LockScreenSettings)
{
    mUi->setupUi(this);
    connect(mUi->lockScreenGroupBox, &QGroupBox::toggled,
            this, &LockScreenSettings::saveSettings);
    connect(mUi->lockCommandLineEdit, &QLineEdit::textEdited,
            this, &LockScreenSettings::saveSettings);
}

LockScreenSettings::~LockScreenSettings()
{
    delete mUi;
}

void LockScreenSettings::loadSettings()
{
    mUi->lockScreenGroupBox->setChecked(mSettings.isLockScreenWatcherEnabled());
    mUi->lockCommandLineEdit->setText(mSettings.customLockScreenCommand());
}

void LockScreenSettings::saveSettings()
{
    mSettings.setLockScreenWatcherEnabled(mUi->lockScreenGroupBox->isChecked());
    mSettings.setCustomLockScreenCommand(mUi->lockCommandLineEdit->text());
}
