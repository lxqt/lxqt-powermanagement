/*
 * File:   Settings.h
 * Author: christian
 *
 * Created on 15. februar 2014, 11:44
 */
#ifndef POWER_MANAGEMENT_SETTINGS_H
#define	POWER_MANAGEMENT_SETTINGS_H

#include <QComboBox>

void fillComboBox(QComboBox* comboBox);

void setComboBoxToValue(QComboBox* comboBox, int value);

int currentValue(QComboBox *comboBox);

#endif // POWER_MANAGEMENT_SETTINGS_H

