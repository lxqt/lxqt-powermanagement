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
#ifndef BATTERY_H
#define BATTERY_H

#include <QObject>
#include <QDBusInterface>
#include <QVariantMap>
#include <QDateTime>

class Battery : public QObject
{
    Q_OBJECT

public:
    // These must match the UPower api spec
    // See http://upower.freedesktop.org/docs/Device.html#Device:State
    enum State
    {
        Unknown = 0,
        Charging,
        Discharging,
        Empty,
        FullyCharged,
        PendingCharge,
        PendingDischarge
    };

    static QList<Battery*> batteries();

    static QString state2String(State state);
    static QString technology2String(int tech);


    Battery(QDBusInterface *deviceInterface, QDBusInterface *propertiesInterface, QObject* parent = 0);
    ~Battery();

    // This data is all available in mProperties, but we keep it
    // here also for easy access (properly converted)
    QString summary;
    QDateTime updated;
    State   state;
    double energyFullDesign;
    double energyFull;
    double capacity;
    double energyNow;
    double energyRate;
    QString model;
    QString technology;
    double voltage;
    double  chargeLevel;
    // --

    bool    haveBattery();

signals:
    void chargeStateChange(float newChargeLevel, Battery::State newState);
    void summaryChanged(QString newSummary);

private slots:
    void uPowerBatteryChanged();

private:
        QDBusInterface *mDeviceInterface;
    QDBusInterface *mPropertiesInterface;
    QVariantMap mProperties;
};
#endif
