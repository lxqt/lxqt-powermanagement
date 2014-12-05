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
#include <QDateTime>

#include "batteryinfo.h"
#include "ui_batteryinfo.h"

BatteryInfo::BatteryInfo(Battery* battery) :
    QFrame(),
    mBattery(battery),
    mUi(new Ui::BatteryInfo)
{
    mUi->setupUi(this);
    connect(mBattery, SIGNAL(batteryChanged()), this, SLOT(onBatteryChanged()));
}

BatteryInfo::~BatteryInfo()
{
    delete mUi;
}

void BatteryInfo::onBatteryChanged()
{
    QDateTime UpdateTime;
    UpdateTime.setTime_t(mBattery->properties().value("UpdateTime").toULongLong());
    mUi->updatedValue->setText(UpdateTime.toString("hh:mm:ss"));

    mUi->stateValue->setText(mBattery->stateAsString());

    QString energyFullDesign("%1 Wh");
    double efd = mBattery->properties().value("EnergyFullDesign").toDouble();
    mUi->energyFullDesignValue->setText(energyFullDesign.arg(efd, 0, 'f', 2));

    QString energyFull("%1 Wh (%2 %)");
    double ef = mBattery->properties().value("EnergyFull").toDouble();
    double capacity = mBattery->properties().value("Capacity").toDouble();
    mUi->energyFullValue->setText(energyFull.arg(ef, 0, 'f', 2).arg(capacity, 0, 'f', 1));

    QString energy("%1 Wh (%2 %)");
    double e = mBattery->properties().value("Energy").toDouble();
    double percentage = mBattery->properties().value("Percentage").toDouble();
    mUi->energyValue->setText(energy.arg(e, 0, 'f', 2).arg(percentage, 0, 'f', 1));

    mUi->energyRateValue->setText(QString::number(mBattery->properties().value("EnergyRate").toDouble(), 'f', 2) + " W");


    mUi->modelValue->setText(mBattery->properties().value("Model").toString());

    int technology = mBattery->properties().value("Technology", 0).toInt();
    switch (technology)
    {
    case 1:  mUi->technologyValue->setText(tr("Lithium ion")); break;
    case 2:  mUi->technologyValue->setText(tr("Lithium polymer")); break;
    case 3:  mUi->technologyValue->setText(tr("Lithium iron phosphate")); break;
    case 4:  mUi->technologyValue->setText(tr("Lead acid")); break;
    case 5:  mUi->technologyValue->setText(tr("Nickel cadmium")); break;
    case 6:  mUi->technologyValue->setText(tr("Nickel metal hydride")); break;
    default: mUi->technologyValue->setText(tr("Unknown")); break;
    }

    mUi->voltageValue->setText(QString::number(mBattery->properties().value("Voltage").toDouble(), 'f', 2) + " V");
}
