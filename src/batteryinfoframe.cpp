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
#include <QDebug>

#include "batteryinfoframe.h"
#include "ui_batteryinfoframe.h"

BatteryInfoFrame::BatteryInfoFrame(Battery *battery) :
    QFrame(),
    mBattery(battery),
    mUi(new Ui::BatteryInfoFrame)
{
    mUi->setupUi(this);

    qDebug() << "Ind i batteryFrame constructor..."
             << "efd:" << mBattery->energyFullDesign
             << "mod:" << mBattery->model
             << "tech:" << mBattery->technology;

    mUi->energyFullDesignValue->setText(mBattery->energyFullDesign);
    mUi->modelValue->setText(mBattery->model);
    mUi->technologyValue->setText(mBattery->technology);

    connect(mBattery, SIGNAL(chargeStateChange(float,Battery::State)), this, SLOT(onBatteryChanged()));
    onBatteryChanged();
}

BatteryInfoFrame::~BatteryInfoFrame()
{
    delete mUi;
}

void BatteryInfoFrame::onBatteryChanged()
{
    qDebug() << "BatteryInfoFrame.onBatteryChanged"
             << "updated:" << mBattery->updated;

    mUi->updatedValue->setText(mBattery->updated);
    mUi->stateValue->setText(mBattery->stateAsText);
    mUi->energyFullValue->setText(mBattery->energyFull);
    mUi->energyValue->setText(mBattery->energyNow);
    mUi->energyRateValue->setText(mBattery->energyRate);
    mUi->voltageValue->setText(mBattery->voltage);
}
