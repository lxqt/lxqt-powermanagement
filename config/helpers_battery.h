/*
* Copyright (c) 2014 Christian Surlykke
*
* This file is part of the LXQt project. <https://lxqt.org>
* It is distributed under the LGPL 2.1 or later license.
* Please refer to the LICENSE file for a copy of the license.
*/
#ifndef POWER_MANAGEMENT_SETTINGS_H
#define	POWER_MANAGEMENT_SETTINGS_H

#include <QComboBox>

void fillComboBoxBat(QComboBox* comboBox);

void setComboBoxToValueBat(QComboBox* comboBox, int value);

int currentValueBat(QComboBox *comboBox);

#endif // POWER_MANAGEMENT_SETTINGS_H

