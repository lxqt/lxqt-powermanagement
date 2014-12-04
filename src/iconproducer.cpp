#include "iconproducer.h"
#include <LXQt/Settings>
#include <XdgIcon>
#include <QDebug>
#include <QtSvg/QSvgRenderer>
#include <QPainter>
#include <math.h>

IconProducer::IconProducer(): mSettings(), mLastThemeName(), mLevelNameMapCharging(), mLevelNameMapDischarging()
{
}


QIcon IconProducer::icon(float chargeLevel, bool discharging)
{
    return mSettings.isUseThemeIcons() ? themedIcon(chargeLevel, discharging) : builtInIcon(chargeLevel, discharging);
}


QIcon IconProducer::themedIcon(float chargeLevel, bool discharging)
{
    return QIcon::fromTheme(iconName(chargeLevel, discharging));
}

QString IconProducer::iconName(float chargeLevel, bool discharging)
{
    if (! mSettings.isUseThemeIcons())
    {
        return QString("Built in: ") + (discharging ? "discharging" : "charging");
    }

    updateLevelNameMaps();

    QMap<float, QString> *levelNameMap =  (discharging ? &mLevelNameMapDischarging : &mLevelNameMapCharging);
    foreach (float level, levelNameMap->keys())
    {
        if (level >= chargeLevel)
        {
            return levelNameMap->value(level);
        }
    }

    return QString();

}


void IconProducer::updateLevelNameMaps()
{
    if (QIcon::themeName() == mLastThemeName) return;

    /*
     * We maintain specific charge-level-to-icon-name mappings for Oxygen and Awoken and
     * asume all other themes adhere to the freedesktop standard.
     * This is certainly not true, and as bug reports come in stating that
     * this and that theme is not working we will probably have to add new
     * mappings beside Oxygen and Awoken
     */
    if (QIcon::themeName() == "oxygen")
    {
        mLevelNameMapCharging[20] = "battery-charging-low";       // Means:
        mLevelNameMapDischarging[20] = "battery-low";             // Use 'battery-low'/'battery-charging-low' for levels up to 20
        mLevelNameMapCharging[40] = "battery-charging-caution";   //
        mLevelNameMapDischarging[40] = "battery-caution";         //  -  'battery-caution'/'battery-charging-caution' for levels between 20 and 40
        mLevelNameMapCharging[60] = "battery-charging-040";       //
        mLevelNameMapDischarging[60] = "battery-040";             // etc..
        mLevelNameMapCharging[80] = "battery-charging-060";
        mLevelNameMapDischarging[80] = "battery-060";
        mLevelNameMapCharging[99] = "battery-charging-080";
        mLevelNameMapDischarging[99] = "battery-080";
        mLevelNameMapCharging[100] = "battery-charging";
        mLevelNameMapDischarging[100] = "battery-100";
    }
    else if (QIcon::themeName() == "awoken")
    {
        mLevelNameMapCharging[20] = "battery-000-charging";
        mLevelNameMapDischarging[20] = "battery-000";
        mLevelNameMapCharging[40] = "battery-020-charging";
        mLevelNameMapDischarging[40] = "battery-020";
        mLevelNameMapCharging[60] = "battery-040-charging";
        mLevelNameMapDischarging[60] = "battery-040";
        mLevelNameMapCharging[80] = "battery-060-charging";
        mLevelNameMapDischarging[80] = "battery-060";
        mLevelNameMapCharging[99] = "battery-080-charging";
        mLevelNameMapDischarging[99] = "battery-080";
        mLevelNameMapCharging[100] = "battery-100-charging";
        mLevelNameMapDischarging[100] = "battery-100";
    }
    else
    {
        mLevelNameMapCharging[1] = "battery-empty";
        mLevelNameMapDischarging[1] = "battery-empty";
        mLevelNameMapCharging[20] = "battery-caution-charging";
        mLevelNameMapDischarging[20] = "battery-caution";
        mLevelNameMapCharging[40] = "battery-low-charging";
        mLevelNameMapDischarging[40] = "battery-low";
        mLevelNameMapCharging[60] = "battery-good-charging";
        mLevelNameMapDischarging[60] = "battery-good";
        mLevelNameMapCharging[100] = "battery-full-charging";
        mLevelNameMapDischarging[100] = "battery-full";
    }

    mLastThemeName = QIcon::themeName();
}


QIcon IconProducer::builtInIcon(float chargeLevel, bool discharging)
{
    float lowLevel = mSettings.getPowerLowLevel();

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
    QSvgRenderer render(svg.toLatin1());
    QPixmap pixmap(render.defaultSize());
    pixmap.fill(QColor(0,0,0,0));
    QPainter painter(&pixmap);
    render.render(&painter);
    return QIcon(pixmap);
}



