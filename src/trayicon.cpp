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

#include <QDebug>
#include <QProcess>
#include <QApplication>
#include <QMessageBox>
#include "trayicon.h"
#include "../config/powermanagementsettings.h"

TrayIcon::TrayIcon(Battery &battery, QObject *parent) : QSystemTrayIcon(parent), contextMenu(), mSettings(), mBattery(battery), mIconProducer()
{
    connect(&mBattery, SIGNAL(batteryChanged()), this, SLOT(batteryChanged()));
    connect(&mSettings, SIGNAL(settingsChanged()), this, SLOT(settingsChanged()));
    connect(LxQt::Settings::globalSettings(), SIGNAL(iconThemeChanged()), this, SLOT(iconThemeChanged()));

    contextMenu.addAction(tr("Configure"), this, SLOT(onConfigureTriggered()));
    contextMenu.addAction(tr("About"), this, SLOT(onAboutTriggered()));
    contextMenu.addAction(tr("Disable icon"), this, SLOT(onDisableIconTriggered()));
    setContextMenu(&contextMenu);
    batteryChanged();
}

TrayIcon::~TrayIcon() {}

void TrayIcon::batteryChanged()
{
    setIcon(mIconProducer.icon(mBattery.chargeLevel(), mBattery.discharging()));
    updateToolTip();
}

void TrayIcon::settingsChanged()
{
    setIcon(mIconProducer.icon(mBattery.chargeLevel(), mBattery.discharging()));
}

void TrayIcon::iconThemeChanged()
{
    setIcon(mIconProducer.icon(mBattery.chargeLevel(), mBattery.discharging()));
}

void TrayIcon::updateToolTip()
{
    QString toolTip = mBattery.discharging() ? tr("discharging") : tr("charging"); // TODO Use states...
    toolTip = toolTip + QString(" - %1 %").arg(mBattery.chargeLevel(), 0, 'f', 1);
    setToolTip(toolTip);
}

void TrayIcon::onConfigureTriggered()
{
    QProcess::startDetached("lxqt-config-powermanagement");
}

void TrayIcon::onAboutTriggered()
{
    QMessageBox::about(0,
                       tr("About"),
                       tr( "<p>"
                           "  <b>LXQt Powermanagement</b><br/>"
                           "  - Powermanagement for the LXQt Desktop Environment"
                           "</p>"
                           "<p>"
                           "  Authors:<br/>"
                           "  &nbsp; Christian Surlykke, Alec Moskvin<br/>"
                           "  &nbsp; - and others from the Razor and LXQt projects"
                           "</p>"
                           "<p>"
                           "  Copyright &copy; 2012-2014"
                           "</p>"
                        ));
}


void TrayIcon::onDisableIconTriggered()
{
    mSettings.setShowIcon(false);
}

