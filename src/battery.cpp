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

#include "battery.h"
#include "../config/powermanagementsettings.h"

Battery::Battery(QObject* parent) 
   : QObject(parent),
     mUPowerBatteryPropertiesInterface(0)
{
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
}

Battery::~Battery()
{
}

void Battery::uPowerBatteryChanged()
{
    if (mUPowerBatteryPropertiesInterface)
    {
        QDBusReply<QVariantMap> reply = mUPowerBatteryPropertiesInterface->call("GetAll", "org.freedesktop.UPower.Device");
        mProperties = reply.value();
        qDebug() << QString("emit batteryChanged(%1, %2);").arg(chargeLevel()).arg(state());
        emit chargeStateChange(chargeLevel(), state());
    }
    else
        qWarning() << "uPowerBatteryProperties has not been initialized";
}


double Battery::chargeLevel()
{
    return mProperties.value("Percentage", 0).toDouble();
}

bool Battery::discharging()
{
    return 2 == state();
}

bool Battery::haveBattery()
{
    return mUPowerBatteryDeviceInterface != 0;
}

Battery::State Battery::state()
{
    return State(mProperties.value("State").toUInt());
}

QString Battery::stateAsString()
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

    return names.value(state());
}

QVariantMap Battery::properties()
{
    return mProperties;
}
