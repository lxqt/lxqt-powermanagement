#include "common.h"

void fillComboBox(QComboBox* comboBox) {
    comboBox->clear();
    comboBox->addItem(QObject::tr("Nothing"), NOTHING);
    comboBox->addItem(QObject::tr("Lock screen"), LOCK);
    comboBox->addItem(QObject::tr("Sleep"), SLEEP);
    comboBox->addItem(QObject::tr("Hibernate"), HIBERNATE);
    comboBox->addItem(QObject::tr("Shutdown"), POWEROFF);
}

void loadValueFromSettings(LxQt::Settings *settings, QComboBox* comboBox, const QString & settingsKey, int defaultValue) {
    int value = settings->value(settingsKey, defaultValue).toInt();

    for (int index = 0; index < comboBox->count(); index++)
    {
        if (value == comboBox->itemData(index).toInt())
        {
            comboBox->setCurrentIndex(index);
            return;
        }
    }

    // Fallback
    comboBox->setCurrentIndex(0);
}

int currentValue(QComboBox *comboBox) {
    return comboBox->itemData(comboBox->currentIndex()).toInt();
}

