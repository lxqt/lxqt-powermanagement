/* 
 * File:   Settings.cpp
 * Author: christian
 * 
 * Created on 15. februar 2014, 11:44
 */
#include <QComboBox>
#include <lxqt/lxqtpower.h>
#include "helpers.h"

void fillComboBox(QComboBox* comboBox)
{
    comboBox->clear();
    comboBox->addItem(QObject::tr("Nothing"), -1);
    comboBox->addItem(QObject::tr("Lock screen"), -2); // FIXME
    comboBox->addItem(QObject::tr("Suspend"), LxQt::Power::PowerSuspend);
    comboBox->addItem(QObject::tr("Hibernate"), LxQt::Power::PowerHibernate);
    comboBox->addItem(QObject::tr("Shutdown"), LxQt::Power::PowerShutdown);
}

void setComboBoxToValue(QComboBox* comboBox, int value)
{
    for (int index = 0; index < comboBox->count(); index++)
    {
        if (value == comboBox->itemData(index).toInt())
        {
            comboBox->setCurrentIndex(index);
            return;
        }
    }

    comboBox->setCurrentIndex(0);
}

int currentValue(QComboBox *comboBox)
{
    return comboBox->itemData(comboBox->currentIndex()).toInt();
}

