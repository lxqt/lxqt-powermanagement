/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXDE-Qt - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2011 Razor team
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
#include <QDBusConnection>
#include <QDBusReply>
#include <QList>
#include <QDBusObjectPath>
#include <QDebug>
#include <QMap>
#include <QDateTime>
#include <math.h>

#include "battery.h"
#include "../config/powermanagementsettings.h"

Battery::Battery(QObject* parent) 
   : QObject(parent),
     mUPowerBatteryPropertiesInterface(0)
{
    // FIXME
    mUPowerInterface = new QDBusInterface("org.freedesktop.UPower", "/org/freedesktop/UPower", "org.freedesktop.UPower", QDBusConnection::systemBus());
    mUPowerBatteryDeviceInterface = 0;
    QDBusReply<QList<QDBusObjectPath> > reply = mUPowerInterface->call("EnumerateDevices");
    foreach (QDBusObjectPath objectPath, reply.value())
    {
        QDBusInterface *device = new QDBusInterface("org.freedesktop.UPower",
                                                    objectPath.path(),
                                                    "org.freedesktop.UPower.Device",
                                                    QDBusConnection::systemBus());

        if (device->property("Type").toUInt() == 2 &&
            ( device->property("PowerSupply").toBool() ||                          // UPower < 0.9.16.3 wrongly reports this false for some laptop batteries
              device->property("NativePath").toString().contains("power_supply"))) // - hence this line
        {
            mUPowerBatteryDeviceInterface = device;
           mUPowerBatteryPropertiesInterface = new QDBusInterface("org.freedesktop.UPower",
                                                         objectPath.path(),
                                                         "org.freedesktop.DBus.Properties",
                                                         QDBusConnection::systemBus());
            if (! connect(mUPowerBatteryDeviceInterface, SIGNAL(Changed()), this, SLOT(uPowerBatteryChanged()))) 
			{
				qDebug() << "Could not connect to 'changed' signal, connecting to PropertiesChanged instead and hoping for the best";
				QDBusConnection::systemBus().connect("org.freedesktop.UPower",
													objectPath.path(),
													"org.freedesktop.DBus.Properties",
													"PropertiesChanged",
													this,
													SLOT(uPowerBatteryChanged()));
			}
			uPowerBatteryChanged();
            break;
        }
        else
        {
            delete device;
        }
    }

    // We assume these do not change during the lifetime of a battery

    energyFullDesign = QString("%1 Wh").arg(mProperties.value("EnergyFullDesign").toDouble(), 0, 'f', 2);
    model = mProperties.value("Model").toString();

    int tech = mProperties.value("Technology", 0).toInt();
    if (tech == 1)  technology = tr("Lithium ion");
    else if (tech == 2)  technology = tr("Lithium polymer");
    else if (tech == 3)  technology = tr("Lithium iron phosphate");
    else if (tech == 4)  technology = tr("Lead acid");
    else if (tech == 5)  technology = tr("Nickel cadmium");
    else if (tech == 6)  technology = tr("Nickel metal hydride");
    else technology = tr("Unknown");

    qDebug() << "Battery created:"
             << "energyFullDesign:" << energyFullDesign
             << "model:" << model
             << "technology" << technology;
}

Battery::~Battery()
{
}

void Battery::uPowerBatteryChanged()
{
    if (mUPowerBatteryPropertiesInterface)
    {
        qDebug() << "uPowerBatteryChanged()";

        QDBusReply<QVariantMap> reply = mUPowerBatteryPropertiesInterface->call("GetAll", "org.freedesktop.UPower.Device");
        mProperties = reply.value();

        State newState = State(mProperties.value("State").toUInt());
        double newChargeLevel = mProperties.value("Percentage").toDouble();

        if (newState == state && fabs(newChargeLevel - chargeLevel) < 0.5)
        {
            // In principle we may miss a change in 'EnergyFull' but we
            // assume it's always accompanied by a change in 'Percentage'
            return;
        }

        chargeLevel =  newChargeLevel;
        state = newState;

        summary = state2String(state);
        if (state == Battery::Charging || state == Battery::Discharging)
        {
            summary.append(QString(" (%1 %)").arg(chargeLevel, 0, 'f', 0));
        }


        updated = QDateTime::fromTime_t(mProperties.value("UpdateTime").toULongLong()).toString("hh:mm:ss");
        stateAsText = state2String(state);
        energyFull = QString("%1 Wh (%2 %)").arg(mProperties.value("EnergyFull").toDouble(), 0, 'f', 2)
                                            .arg(mProperties.value("Capacity").toDouble(), 0, 'f', 1);

        energyNow = QString("%1 Wh (%2 %)").arg(mProperties.value("Energy").toDouble(), 0, 'f', 2)
                                           .arg(mProperties.value("Percentage").toDouble(), 0, 'f', 1);

        energyRate = QString("%1 W").arg(mProperties.value("EnergyRate").toDouble(), 0, 'f', 2);
        voltage = QString("%1 V").arg(mProperties.value("Voltage").toDouble(), 0, 'f', 2);

        qDebug() << "battery now:"
                 << "summary" << summary
                 << "updated:" << updated
                 << "stateAsText:" << stateAsText
                 << "energyFull:" << energyFull
                 << "energyNow:" << energyNow
                 << "energyRate:" << energyRate
                 << "voltage:" << voltage;

        emit chargeStateChange(chargeLevel, state);
        emit summaryChanged(summary);
    }
    else
        qWarning() << "uPowerBatteryProperties has not been initialized";
}


bool Battery::haveBattery()
{
    return mUPowerBatteryDeviceInterface != 0;
}

QString Battery::state2String(State state)
{
    static QMap<State, QString> names;
    if (names.isEmpty())
    {
        names.insert(Unknown, tr("Unknown"));
        names.insert(Charging, tr("Charging"));
        names.insert(Discharging, tr("Discharging"));
        names.insert(Empty, tr("Empty"));
        names.insert(FullyCharged, tr("Fully charged"));
        names.insert(PendingCharge, tr("Pending charge"));
        names.insert(PendingDischarge, tr("Pending discharge"));
    }

    return names.value(state);
}
