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

#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>
#include <QMenu>

#include "batteryinfo.h"
#include "battery.h"

class IconNamingScheme 
{
public:
    static IconNamingScheme* getNamingSchemeForCurrentIconTheme();
    QString iconName(float chargeLevel, bool discharging) const;
    bool isValidForCurrentIconTheme() const;

private:
    IconNamingScheme(QString schemeName, QList<float> chargeLevels, QList<QString> iconNamesCharging, QList<QString> iconNamesDischarging);

    QList<float> mChargeLevels;
    QList<QString> mIconNamesCharging;
    QList<QString> mIconNamesDischarging;
    QString mSchemeName;
};


class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

public:
    TrayIcon(QObject *parent = 0);
    ~TrayIcon();

public:
    void update(bool discharging, double chargeLevel, double lowLevel);
    virtual bool isProperForCurrentSettings() = 0;

protected:
    void updateToolTip();
    virtual void updateIcon() = 0;
    bool discharging;
    double chargeLevel;
    double lowLevel;

private slots:
    void onAboutTriggered();
    void onConfigureTriggered();

private:
    QMenu contextMenu;
};

class TrayIconBuiltIn : public TrayIcon 
{
    Q_OBJECT

public:
    TrayIconBuiltIn(QObject *parent = 0);
    ~TrayIconBuiltIn();
    
    virtual bool isProperForCurrentSettings();

protected:
    virtual void updateIcon();
    
};

class TrayIconTheme : public TrayIcon
{
    Q_OBJECT

public:
    TrayIconTheme(IconNamingScheme *iconNamingScheme, QObject* parent);
    ~TrayIconTheme();

    virtual bool isProperForCurrentSettings();

protected:
    virtual void updateIcon();
    
private:
    IconNamingScheme *mIconNamingScheme;

};

#endif // TRAYICON_H
