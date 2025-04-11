/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXQt - a lightweight, Qt based, desktop toolset
 * https://lxqt.org
 *
 * Copyright: 2011 Razor team
 *            2025~ LXQt team
 * Authors:
 *   Christian Surlykke <christian@surlykke.dk>
 *   Palo Kisa <palo.kisa@gmail.com>
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

#include <Solid/Battery>
#include <Solid/Device>
#include <QStringBuilder>

#include "trayiconbattery.h"
#include "batteryhelper.h"

TrayIconBattery::TrayIconBattery(Solid::Battery *battery, QObject *parent)
    : TrayIcon(parent),
    mBattery(battery),
    mIconProducer(battery)
{
    connect(mBattery, &Solid::Battery::chargePercentChanged, this, &TrayIconBattery::updateTooltip);
    connect(mBattery, &Solid::Battery::chargeStateChanged, this, &TrayIconBattery::updateTooltip);
    connect(mBattery, &Solid::Battery::timeToEmptyChanged, this, &::TrayIconBattery::updateTooltip);
    connect(mBattery, &Solid::Battery::timeToFullChanged, this, &::TrayIconBattery::updateTooltip);
    updateTooltip();

    connect(&mIconProducer, &IconProducer::iconChanged, this, &TrayIcon::iconChanged);
    iconChanged();

}

TrayIconBattery::~TrayIconBattery()
{
}

/*virtual*/ const QIcon & TrayIconBattery::getIcon() const
{
    return mIconProducer.mIcon;
}

void TrayIconBattery::updateTooltip()
{
    QString stateStr = mBattery->chargePercent() <= 0 && mBattery->chargeState() == Solid::Battery::NoCharge ?
                       tr("Empty") : BatteryHelper::stateToString(mBattery->chargeState());
    QString tooltip = stateStr % QString::fromLatin1(" (%1%)").arg(mBattery->chargePercent());
    switch (mBattery->chargeState())
    {
        case Solid::Battery::Charging:
            tooltip += QL1S(", ") % BatteryHelper::timeToFullString(mBattery->timeToFull());
            break;
        case Solid::Battery::Discharging:
            tooltip += QL1S(", ") % BatteryHelper::timeToEmptyString(mBattery->timeToEmpty());
            break;
        default:
            break;
    }

    setToolTip(tooltip);
}
