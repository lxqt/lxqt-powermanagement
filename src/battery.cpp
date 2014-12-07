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

#define UPOWER_SERVICE "org.freedesktop.UPower"
#define UPOWER_PATH "/org/freedesktop/UPower"
#define UPOWER_INTERFACE "org.freedesktop.UPower"
#define DEVICE_INTERFACE "org.freedesktop.UPower.Device"
#define PROPERTIES_INTERFACE "org.freedesktop.DBus.Properties"

QList<Battery*> Battery::batteries()
{
    QList<Battery*> result;
    QDBusInterface uPowerInterface(UPOWER_SERVICE, UPOWER_PATH, UPOWER_SERVICE, QDBusConnection::systemBus());

    QDBusReply<QList<QDBusObjectPath> > reply = uPowerInterface.call("EnumerateDevices");
    foreach (QDBusObjectPath objectPath, reply.value())
    {
        QDBusInterface *deviceInterface = new QDBusInterface(UPOWER_SERVICE,  objectPath.path(), DEVICE_INTERFACE, QDBusConnection::systemBus());

        if (deviceInterface->property("Type").toUInt() == 2 &&
            ( deviceInterface->property("PowerSupply").toBool() ||                          // UPower < 0.9.16.3 wrongly reports this false for some laptop batteries
              deviceInterface->property("NativePath").toString().contains("power_supply"))) // - hence this line
        {
            QDBusInterface *propertiesInterface = new QDBusInterface(UPOWER_SERVICE, objectPath.path(), PROPERTIES_INTERFACE,  QDBusConnection::systemBus());

            result << new Battery(deviceInterface, propertiesInterface);
        }
        else
        {
            delete deviceInterface;
        }
    }

    return result;

}

Battery::Battery(QDBusInterface *deviceInterface, QDBusInterface *propertiesInterface, QObject* parent)
   : QObject(parent), mDeviceInterface(deviceInterface), mPropertiesInterface(propertiesInterface)
{
    if (! connect(mDeviceInterface, SIGNAL(Changed()), this, SLOT(uPowerBatteryChanged())) )
    {
        qDebug() << "Could not connect to 'changed' signal, connecting to PropertiesChanged instead and hoping for the best";
        QDBusConnection::systemBus().connect(UPOWER_SERVICE,
                                             mPropertiesInterface->path(),
                                             PROPERTIES_INTERFACE,
                                             "PropertiesChanged",
                                             this,
                                             SLOT(uPowerBatteryChanged()));

        connect(mPropertiesInterface, SIGNAL(PropertiesChanged()), this, SLOT(uPowerBatteryChanged()));
    }

    uPowerBatteryChanged(); // To get all properties

    // We assume these do not change during the lifetime of a battery
    energyFullDesign = mProperties.value("EnergyFullDesign").toDouble();
    model = mProperties.value("Model").toString();
    technology = technology2String(mProperties.value("Technology, 0").toInt());
}

Battery::~Battery()
{
}

void Battery::uPowerBatteryChanged()
{
    qDebug() << "uPowerBatteryChanged()";

    QDBusReply<QVariantMap> reply = mPropertiesInterface->call("GetAll", "org.freedesktop.UPower.Device");
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


    updated = QDateTime::fromTime_t(mProperties.value("UpdateTime").toULongLong());
    energyFull = mProperties.value("EnergyFull").toDouble();
    capacity = mProperties.value("Capacity").toDouble();
    energyNow = mProperties.value("Energy").toDouble();
    energyRate = mProperties.value("EnergyRate").toDouble();
    voltage = mProperties.value("Voltage").toDouble();

    qDebug() << "battery now:"
             << "summary" << summary
             << "updated:" << updated
             << "energyFull:" << energyFull
             << "energyNow:" << energyNow
             << "energyRate:" << energyRate
             << "voltage:" << voltage;

    emit chargeStateChange(chargeLevel, state);
    emit summaryChanged(summary);
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

QString Battery::technology2String(int tech)
{
    switch (tech)
      {
    case 1: return tr("Lithium ion");
    case 2: return tr("Lithium polymer");
    case 3: return tr("Lithium iron phosphate");
    case 4: return tr("Lead acid");
    case 5: return tr("Nickel cadmium");
    case 6: return tr("Nickel metal hydride");
    default: return tr("Unknown");
    }
}
