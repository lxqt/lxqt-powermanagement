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

#include <math.h>
#include <QDebug>
#include <QtSvg/QSvgRenderer>
#include <QtGui/QPainter>
#include <lxqt/lxqtsettings.h>

#include "trayicon.h"
#include "../config/common.h"

QString IconNamingScheme::iconName(float chargeLevel, bool discharging) const
{   
    int i = 0;
    while(i < mChargeLevels.size() - 1 && mChargeLevels.at(i+1) <= chargeLevel) i++;
    
    return discharging ? mIconNamesDischarging.at(i) : mIconNamesCharging.at(i);
}


IconNamingScheme::IconNamingScheme(QString schemeName, QList<float> chargeLevels, QList<QString> iconNamesCharging, QList<QString> iconNamesDischarging)
{
    mSchemeName = schemeName;
    mChargeLevels = chargeLevels;
    mIconNamesCharging = iconNamesCharging;
    mIconNamesDischarging = iconNamesDischarging;
}

IconNamingScheme* IconNamingScheme::getNamingSchemeForCurrentIconTheme()
{
    static QList<IconNamingScheme*> schemes;
    
    if (schemes.isEmpty()) 
    {
        // Freedesktop naming scheme 
         schemes << new IconNamingScheme(
                "freedesktop",
                (QList<float>() << 0 << 1 << 20 << 40 << 60),
                (QList<QString>() << "battery-empty" << "battery-caution-charging" << "battery-low-charging" 
                                  << "battery-good-charging" <<  "battery-full-charging"),
                (QList<QString>() << "battery-empty" << "battery-caution" << "battery-low" << "battery-good" <<  "battery-full")
                );
        
         // Oxygen naming scheme
         schemes << new IconNamingScheme(
                "oxygen",
                ((QList<float>()) << 0 << 20 << 40 << 60 << 80 << 99.5),
                ((QList<QString>()) << "battery-charging-low" << "battery-charging-caution" << "battery-charging-040"
                                    << "battery-charging-060" << "battery-charging-080" << "battery-charging"),
                ((QList<QString>()) << "battery-low" << "battery-caution" << "battery-040"
                                    << "battery-060" << "battery-080" << "battery-100")
                );

         // AwOken naming scheme
         schemes << new IconNamingScheme(
                "AwOken",
                ((QList<float>()) << 0 << 20 << 40 << 60 << 80 << 99.5),
                ((QList<QString>()) << "battery-000-charging" << "battery-020-charging" << "battery-040-charging"
                                    << "battery-060-charging" << "battery-080-charging" << "battery-100-charging"),
                ((QList<QString>()) << "battery-000" << "battery-020" << "battery-040"
                                   << "battery-060" << "battery-080" << "battery-100")
                );
    }

    foreach (IconNamingScheme* iconNamingScheme, schemes)
    {
        if (iconNamingScheme->isValidForCurrentIconTheme())
        {
            qDebug() << "Found IconNamingScheme:" << iconNamingScheme->mSchemeName;
            return iconNamingScheme;
        }
    }

    qDebug() << "Found no IconNamingScheme";
    return 0;
}


bool IconNamingScheme::isValidForCurrentIconTheme() const
{
    for (int i = 0; i < mIconNamesCharging.size(); i++) 
    {
        qDebug() << "Looking for"  << mIconNamesCharging.at(i);
        if (!QIcon::hasThemeIcon(mIconNamesCharging.at(i)))
        {
            qDebug() << "Not found";
            return false;
        }
        
        qDebug() << "Looking for"  << mIconNamesDischarging.at(i);
        if (!QIcon::hasThemeIcon(mIconNamesDischarging.at(i)))
        {
            qDebug() << "Not found";
            return false;
        }
    }
    qDebug();

    return true;
}


TrayIcon::TrayIcon(QObject *parent) : QSystemTrayIcon(parent) {}

TrayIcon::~TrayIcon() {}

void TrayIcon::update(bool discharging, double chargeLevel, double lowLevel)
{
    this->discharging = discharging;
    this->chargeLevel = chargeLevel;
    this->lowLevel = lowLevel;

    updateToolTip();
    updateIcon();
}



void TrayIcon::updateToolTip()
{
    QString toolTip = discharging ? tr("discharging") : tr("charging"); // TODO Use states...
    toolTip = toolTip + QString(" - %1 %").arg(chargeLevel, 0, 'f', 1);
    setToolTip(toolTip);
}


TrayIconBuiltIn::TrayIconBuiltIn(QObject* parent) : 
    TrayIcon(parent)
{
}

TrayIconBuiltIn::~TrayIconBuiltIn()
{
}


void TrayIconBuiltIn::updateIcon()
{
    // See http://www.w3.org/TR/SVG/Overview.html 
    // and regarding circle-arch in particular: 
    // http://www.w3.org/TR/SVG/paths.html#PathDataEllipticalArcCommands
   
    // We show charge with a segment of a circle.
    // We start at the top of the circle
    // The starting point of the circle segment is at the top (12 o'clock or (0,1) or pi/2 
    // and it moves counter-clockwise as the charge increases
    // First we calculate in floating point numbers, using a circle with center
    // in (0,0) and a radius of 1
    double angle = 2*M_PI*chargeLevel/100 + M_PI_2;
    qDebug() << "Angle:"  << angle;
    double segment_endpoint_x = cos(angle);
    double segment_endpoint_y = sin(angle);

    // svg uses a coordinate system with (0,0) at the topmost left corner,  
    // y increasing downwards and x increasing left-to-right.
    // We draw the circle segments with center at (100,100) and radius 100 for the
    // outer and radius 60 for the inner. The segments will (unless fully charged, 
    // where they go full circle) be radially connected at the endpoints.
    QString chargeGraphics;

    if (chargeLevel < 0.5)
    {
        chargeGraphics = "";
    }
    else if (chargeLevel > 99.5)
    {
        chargeGraphics =
            "<path d='M 100,0 A 100,100 0 1,0 101,0 z M 100,40 A 60,60 0 1,0 101,40 z' stroke-width='0'/>";
    }
    else {
        chargeGraphics = 
            QString("<path d='M 100,0 A 100,100 0 %1,0 %2,%3 L %4,%5 A 60,60 0 %1,1 100,40 z'/>")
                .arg(angle > M_PI + M_PI_2 ? 1 : 0)     // %1 
                .arg(round(100*(1 + segment_endpoint_x)))    // %2
                .arg(round(100*(1 - segment_endpoint_y)))    // %3
                .arg(round(100*(1 + 0.6*segment_endpoint_x)))    // %4
                .arg(round(100*(1 - 0.6*segment_endpoint_y)));   // %5
    }
 
    QString chargeColor;
    if (discharging && chargeLevel <= lowLevel + 10)
    {
        chargeColor = "rgb(200,40,40)";
    }
    else if (discharging && chargeLevel <= lowLevel + 30) 
    {
        int fac = chargeLevel - lowLevel - 10;
        chargeColor = QString("rgb(%1,%2,40)").arg(40 + 200 - fac*8).arg(40 + fac*8);
    }
    else 
        chargeColor = "rgb(40,200,40)";


    QString sign = 
            discharging ? 
                QString("<path d='M 60,100 h 80' stroke='black' stroke-width='20' />"):                // Minus
                QString("<path d='M 60,100 h 80 M 100,60 v 80' stroke='black' stroke-width='20' />");  // Plus

    QString svg = 
        QString("<?xml version='1.0' standalone='no'?>\n"
                "<!DOCTYPE svg PUBLIC '-//W3C//DTD SVG 1.1//EN' 'http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd'>\n"
                "<svg xmlns='http://www.w3.org/2000/svg'  version='1.1' viewBox='0 0 210 210'>\n"
                "    <circle cx='100' cy='100' r='100' fill='rgb(210,210,210)' stroke-width='0'/>\n"
                "    <circle cx='100' cy='100' r='55' fill='rgb(255,255,255)' stroke-width='0'/>\n"
                "    <g fill-rule='evenodd' fill='%2' stroke-width='0'>\n"
                "        %1\n"
                "    </g>\n"
                "    %3\n" 
                "</svg>\n").arg(chargeGraphics).arg(chargeColor).arg(sign);

    qDebug() << svg;

    // Paint the svg on a pixmap and create an icon from that.
    QSvgRenderer render(svg.toAscii());
    QPixmap pixmap(render.defaultSize());
    pixmap.fill(QColor(0,0,0,0));
    QPainter painter(&pixmap);
    render.render(&painter);
    setIcon(QIcon(pixmap));
}

TrayIconTheme::TrayIconTheme(IconNamingScheme* iconNamingScheme, QObject* parent)
{
    this->mIconNamingScheme = iconNamingScheme;
}

TrayIconTheme::~TrayIconTheme()
{
}

bool TrayIconTheme::isProperForCurrentSettings(bool useThemedIcons)
{
    return useThemedIcons && mIconNamingScheme->isValidForCurrentIconTheme();
}



void TrayIconTheme::updateIcon()
{
    QString iconName = mIconNamingScheme->iconName(chargeLevel, discharging);
    setIcon(QIcon::fromTheme(iconName));

}
