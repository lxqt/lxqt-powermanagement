/*
* Copyright (c) 2014, 2015 Christian Surlykke, Paulo Lieuthier
*
* This file is part of the LXQt project. <https://lxqt.org>
* It is distributed under the LGPL 2.1 or later license.
* Please refer to the LICENSE file for a copy of the license.
*/
#include "iconproducer.h"

#include <LXQt/Globals>
#include <LXQt/Settings>
#include <XdgIcon>
#include <QDebug>
#include <QtSvg/QSvgRenderer>
#include <QPixmapCache>
#include <QPainter>
#include <QStringBuilder>
#include <cmath>
#include <tuple>

IconProducer::IconProducer(Solid::Battery *battery, QObject *parent) : QObject(parent)
{
    connect(battery, &Solid::Battery::chargeStateChanged, this, &IconProducer::updateState);
    connect(battery, &Solid::Battery::chargePercentChanged, this, &IconProducer::updateChargePercent);
    connect(&mSettings, &PowerManagementSettings::settingsChanged, this, &IconProducer::update);

    mChargePercent = battery->chargePercent();
    mState = battery->chargeState();
    themeChanged();
}

IconProducer::IconProducer(QObject *parent):  QObject(parent)
{
    themeChanged();
    update();
}


void IconProducer::updateChargePercent(int newChargePercent)
{
    mChargePercent = newChargePercent;

    update();
}

void IconProducer::updateState(int newState)
{
    mState = (Solid::Battery::ChargeState) newState;

    update();
}

void IconProducer::update()
{
    QString newIconName;

    if (mSettings.isUseThemeIcons())
    {
        QMap<float, QString> *levelNameMap = (mState == Solid::Battery::Discharging ? &mLevelNameMapDischarging : &mLevelNameMapCharging);
        const auto levels = levelNameMap->keys();
        for (const float level : levels)
        {
            if (level >= mChargePercent)
            {
                newIconName = levelNameMap->value(level);
                break;
            }
        }
    }

    if (mSettings.isUseThemeIcons() && newIconName == mIconName)
        return;

    mIconName = newIconName;
    mIcon = mSettings.isUseThemeIcons() ? QIcon::fromTheme(mIconName) : generatedIcon();
    emit iconChanged();
}

void IconProducer::themeChanged()
{
    /*
     * We maintain specific charge-level-to-icon-name mappings for Oxygen and Awoken and
     * asume all other themes adhere to the freedesktop standard.
     * This is certainly not true, and as bug reports come in stating that
     * this and that theme is not working we will probably have to add new
     * mappings beside Oxygen and Awoken
     */
    mLevelNameMapDischarging.clear();
    mLevelNameMapCharging.clear();

    if (QIcon::themeName() == QL1S("oxygen"))
    {
        // Means:
        // Use 'battery-low' for levels up to 10
        //  -  'battery-caution' for levels between 10 and 20
        //  -  'battery-040' for levels between 20 and 40, etc..
        mLevelNameMapDischarging[10] = QL1S("battery-low");
        mLevelNameMapDischarging[20] = QL1S("battery-caution");
        mLevelNameMapDischarging[40] = QL1S("battery-040");
        mLevelNameMapDischarging[60] = QL1S("battery-060");
        mLevelNameMapDischarging[80] = QL1S("battery-080");
        mLevelNameMapDischarging[101] = QL1S("battery-100");
        mLevelNameMapCharging[10] = QL1S("battery-charging-low");
        mLevelNameMapCharging[20] = QL1S("battery-charging-caution");
        mLevelNameMapCharging[40] = QL1S("battery-charging-040");
        mLevelNameMapCharging[60] = QL1S("battery-charging-060");
        mLevelNameMapCharging[80] = QL1S("battery-charging-080");
        mLevelNameMapCharging[101] = QL1S("battery-charging");
    }
    else if (QIcon::themeName().startsWith(QL1S("AwOken"))) // AwOken, AwOkenWhite, AwOkenDark
    {
        mLevelNameMapDischarging[5] = QL1S("battery-000");
        mLevelNameMapDischarging[30] = QL1S("battery-020");
        mLevelNameMapDischarging[50] = QL1S("battery-040");
        mLevelNameMapDischarging[70] = QL1S("battery-060");
        mLevelNameMapDischarging[95] = QL1S("battery-080");
        mLevelNameMapDischarging[101] = QL1S("battery-100");
        mLevelNameMapCharging[5] = QL1S("battery-000-charging");
        mLevelNameMapCharging[30] = QL1S("battery-020-charging");
        mLevelNameMapCharging[50] = QL1S("battery-040-charging");
        mLevelNameMapCharging[70] = QL1S("battery-060-charging");
        mLevelNameMapCharging[95] = QL1S("battery-080-charging");
        mLevelNameMapCharging[101] = QL1S("battery-100-charging");
    }
    else // As default we fall back to the freedesktop scheme.
    {
        mLevelNameMapDischarging[3] = QL1S("battery-empty");
        mLevelNameMapDischarging[10] = QL1S("battery-caution");
        mLevelNameMapDischarging[50] = QL1S("battery-low");
        mLevelNameMapDischarging[90] = QL1S("battery-good");
        mLevelNameMapDischarging[101] = QL1S("battery-full");
        mLevelNameMapCharging[3] = QL1S("battery-empty");
        mLevelNameMapCharging[10] = QL1S("battery-caution-charging");
        mLevelNameMapCharging[50] = QL1S("battery-low-charging");
        mLevelNameMapCharging[90] = QL1S("battery-good-charging");
        mLevelNameMapCharging[101] = QL1S("battery-full-charging");
    }

    update();
}

template <>
QString IconProducer::buildIcon<PowerManagementSettings::ICON_CIRCLE>(Solid::Battery::ChargeState state, int chargeLevel)
{
    static QString svg_template = QL1S(
        "<svg\n"
        "    xmlns='http://www.w3.org/2000/svg'\n"
        "    version='1.1'\n"
        "    width='200'\n"
        "    height='200'>\n"
        "\n"
        "<defs>\n"
        "    <linearGradient id='greenGradient' x1='0%' y1='0%' x2='100%' y2='100%'>\n"
        "        <stop offset='0%' style='stop-color:rgb(125,255,125);stop-opacity:1' />\n"
        "        <stop offset='150%' style='stop-color:rgb(15,125,15);stop-opacity:1' />\n"
        "    </linearGradient>\n"
        "</defs>\n"
        "\n"
        "<circle cx='100' cy='100' r='99' style='fill:#FFFFFF;stroke:none; opacity:0.2;'/>\n"
        "<path d='M 100,20 A80,80 0, LARGE_ARC_FLAG, SWEEP_FLAG, END_X,END_Y' style='fill:none; stroke:url(#greenGradient); stroke-width:38;' />\n"
        "<path d='M 100,20 A80,80 0, LARGE_ARC_FLAG, SWEEP_FLAG, END_X,END_Y' style='fill:none; stroke:red; stroke-width:38; opacity:RED_OPACITY' />\n"
        "\n"
        " STATE_MARKER\n"
        "\n"
        "</svg>");

    static QString filledCircle   = QL1S("<circle cx='100' cy='100' r='35'/>");
    static QString plus           = QL1S("<path d='M 60,100 L140,100 M100,60 L100,140' style='stroke:black; stroke-width:30;'/>");
    static QString minus          = QL1S("<path d='M 60,100 L140,100' style='stroke:black; stroke-width:30;'/>");
    static QString hollowCircle   = QL1S("<circle cx='100' cy='100' r='30' style='fill:none;stroke:black;stroke-width:10'/>");

    QString svg = svg_template;

    if (chargeLevel > 99)
        chargeLevel = 99;

    double angle;
    QString sweepFlag;
    if (state == Solid::Battery::Discharging)
    {
        angle = M_PI_2 + 2 * M_PI * chargeLevel/100;
        sweepFlag = QL1C('0');
    }
    else
    {
        angle = M_PI_2 - 2 *M_PI * chargeLevel/100;
        sweepFlag = QL1C('1');
    }
    double circle_endpoint_x = 80.0 * cos(angle) + 100;
    double circle_endpoint_y = -80.0 * sin(angle) + 100;

    QString largeArgFlag = chargeLevel > 50 ? QL1S("1") : QL1S("0");

    svg.replace(QL1S("END_X"), QString::number(circle_endpoint_x));
    svg.replace(QL1S("END_Y"), QString::number(circle_endpoint_y));
    svg.replace(QL1S("LARGE_ARC_FLAG"), largeArgFlag);
    svg.replace(QL1S("SWEEP_FLAG"), sweepFlag);

    switch (state)
    {
    case Solid::Battery::FullyCharged:
        svg.replace(QL1S("STATE_MARKER"), filledCircle);
        break;
    case Solid::Battery::Charging:
        svg.replace(QL1S("STATE_MARKER"), plus);
        break;
    case Solid::Battery::Discharging:
        svg.replace(QL1S("STATE_MARKER"), minus);
        break;
    default:
        svg.replace(QL1S("STATE_MARKER"), hollowCircle);
    }

    if (state != Solid::Battery::FullyCharged && state != Solid::Battery::Charging &&  chargeLevel < mSettings.getPowerLowLevel() + 30)
    {
        if (chargeLevel <= mSettings.getPowerLowLevel() + 10)
            svg.replace(QL1S("RED_OPACITY"), QL1S("1"));
        else
            svg.replace(QL1S("RED_OPACITY"), QString::number((mSettings.getPowerLowLevel() + 30 - chargeLevel)/20.0));
    }
    else
        svg.replace(QL1S("RED_OPACITY"), QL1S("0"));

    return svg;
}

template <>
QString IconProducer::buildIcon<PowerManagementSettings::ICON_CIRCLE_ENHANCED>(Solid::Battery::ChargeState state, int chargeLevel)
{
    static QString svg_template = QL1S(
        "<svg\n"
        "    xmlns='http://www.w3.org/2000/svg'\n"
        "    version='1.1'\n"
        "    width='200'\n"
        "    height='200'>\n"
        "\n"
        "<defs>\n"
        "    <linearGradient id='greenGradient' x1='0%' y1='0%' x2='100%' y2='100%'>\n"
        "        <stop offset='0%' style='stop-color:rgb(125,255,125);stop-opacity:1' />\n"
        "        <stop offset='150%' style='stop-color:rgb(15,125,15);stop-opacity:1' />\n"
        "    </linearGradient>\n"
        "</defs>\n"
        "\n"
        "<rect x='0' y='0' width='200' height='200' rx='30' style='fill:white;opacity:0.7;'/>\n"
        "ARC_LEVEL\n"
        "STATE_MARKER\n"
        "<text x='100' y='130' text-anchor='middle' font-size='80' font-weight='bold' fill='black'>PERCENT</text>\n"
        "</svg>");

    static QString levelArcs      = QL1S(
        "<path d='M 100,20 A80,80 0, LARGE_ARC_FLAG, SWEEP_FLAG, END_X,END_Y' style='fill:none; stroke:url(#greenGradient); stroke-width:38;' />\n"
        "<path d='M 100,20 A80,80 0, LARGE_ARC_FLAG, SWEEP_FLAG, END_X,END_Y' style='fill:none; stroke:red; stroke-width:38; opacity:RED_OPACITY' />\n");
    static QString levelCircle    = QL1S("<circle cx='100' cy='100' r='80' style='fill:none; stroke:url(#greenGradient); stroke-width:38;' />");
    static QString star   = QL1S("<path style='fill:yellow' d='m 118.5833,57.0395 -18.3781,-13.4538 -18.7005,13.4978 7.1079,-21.6316 -18.6126,-13.615 22.7748,.0733 7.1959,-21.9101 6.9614,21.6756 23.0679,.0879 -18.466,13.322 Z '/>");
    static QString plus           = QL1S("<path d='M 10,35 L60,35 M35,10 L35,60' style='stroke:#346334; stroke-width:15;'/>");
    static QString minus          = QL1S("<path d='M 135,30 L185,30' style='stroke:#a82424; stroke-width:15;'/>");

    QString svg = svg_template;

    if (chargeLevel > 99)
    {
      svg.replace(QL1S("ARC_LEVEL"), levelCircle);
    } else
    {
      svg.replace(QL1S("ARC_LEVEL"), levelArcs);
      double angle;
      QString sweepFlag;
      if (state == Solid::Battery::Discharging)
      {
        angle = M_PI_2 + 2 * M_PI * chargeLevel/100;
        sweepFlag = QL1C('0');
      }
      else
      {
        angle = M_PI_2 - 2 *M_PI * chargeLevel/100;
        sweepFlag = QL1C('1');
      }
      double circle_endpoint_x = 80.0 * cos(angle) + 100;
      double circle_endpoint_y = -80.0 * sin(angle) + 100;

      QString largeArgFlag = chargeLevel > 50 ? QL1S("1") : QL1S("0");

      svg.replace(QL1S("END_X"), QString::number(circle_endpoint_x));
      svg.replace(QL1S("END_Y"), QString::number(circle_endpoint_y));
      svg.replace(QL1S("LARGE_ARC_FLAG"), largeArgFlag);
      svg.replace(QL1S("SWEEP_FLAG"), sweepFlag);
    }
    svg.replace(QL1S("PERCENT"), QString::number(chargeLevel));

    switch (state)
    {
    case Solid::Battery::FullyCharged:
        svg.replace(QL1S("STATE_MARKER"), star);
        break;
    case Solid::Battery::Charging:
        svg.replace(QL1S("STATE_MARKER"), plus);
        break;
    case Solid::Battery::Discharging:
        svg.replace(QL1S("STATE_MARKER"), minus);
        break;
    default:
        svg.replace(QL1S("STATE_MARKER"), QString{});
    }

    if (state != Solid::Battery::FullyCharged && state != Solid::Battery::Charging &&  chargeLevel < mSettings.getPowerLowLevel() + 30)
    {
        if (chargeLevel <= mSettings.getPowerLowLevel() + 10)
            svg.replace(QL1S("RED_OPACITY"), QL1S("1"));
        else
            svg.replace(QL1S("RED_OPACITY"), QString::number((mSettings.getPowerLowLevel() + 30 - chargeLevel)/20.0));
    }
    else
        svg.replace(QL1S("RED_OPACITY"), QL1S("0"));

    return svg;
}

static QString buildBatteryIcon(Solid::Battery::ChargeState state, int chargeLevel, bool opaque)
{
    const double red_opacity = state == Solid::Battery::Charging ? 0.0 : qBound(0.0, 1 - ((chargeLevel + 70) / 100 + static_cast<double>(chargeLevel) / 30), 1.0);
    return QL1S("<svg xmlns='http://www.w3.org/2000/svg' version='1.1' width='130' height='130' viewBox='0 0 130 130'>\n"
        "\n"
        "<defs>\n"
        "  <linearGradient id='greenGradient' x1='0' y1='-30' x2='0' y2='30' gradientUnits='userSpaceOnUse'>\n"
        "    <stop offset='0%' style='stop-color:rgb(125,255,125);") % (opaque ? QString{} : QL1S("stop-opacity:0.7")) % QL1S("' />\n"
        "    <stop offset='100%' style='stop-color:rgb(15,125,15);") % (opaque ? QString{} : QL1S("stop-opacity:0.7")) % QL1S("' />\n"
        "  </linearGradient>\n"
        "</defs>\n"
        "\n")
        % (opaque ? QString{} : QL1S("<rect x='0' y='0' width='130' height='130' rx='20' style='fill:white;opacity:0.7;'/>\n"))
        % QL1S("<g transform='translate(14 65)'>\n"
        "  <path d='M -2 -32 h 104 v 23 h 6 v 18 h -6 v 23 h -104 z' style='stroke:rgb(113,193,113); stroke-width:4; stroke-linejoin: round; fill:lightgrey;") % (opaque ? QL1S("") : QL1S(" opacity:0.7;")) % QL1S("'/>\n"
        "  <rect x='0' y='-30' width='") % QString::number(chargeLevel) % QL1S("' height='60' style='fill:url(#greenGradient);'/>\n")
        % (red_opacity > 0.0
                ? QL1S("  <rect x='0' y='-30' width='") % QString::number(chargeLevel) % QL1S("' height='60' style='fill:red; opacity:") % QString::number(red_opacity) % QL1S("'/>\n")
                : QString{}
          )
        % QL1S("  <text x='50' y='18' text-anchor='middle' font-size='50' font-weight='bold' fill='black'>") % QString::number(chargeLevel) % QL1S("</text>\n"
        "</g>\n")
        % ((state == Solid::Battery::Charging || state == Solid::Battery::FullyCharged)
                ? QL1S("<path d='M 127.5,30 68,13.5 v 14 L 5.5,9 l 54,5 V 0 Z' style='fill:#ffc800'/>\n"
                       "<path d='m 127.5,30 -50,-4 V 42.5 L 5.5,9 68,27.5 v -14 z' style='fill:#dc3800'/>\n")
                : QString{}
          )
        % QL1S("</svg>");
}

template <>
QString IconProducer::buildIcon<PowerManagementSettings::ICON_BATTERY>(Solid::Battery::ChargeState state, int chargeLevel)
{
    return buildBatteryIcon(state, chargeLevel, false);
}

template <>
QString IconProducer::buildIcon<PowerManagementSettings::ICON_BATTERY_OPAQUE>(Solid::Battery::ChargeState state, int chargeLevel)
{
    return buildBatteryIcon(state, chargeLevel, true);
}

QIcon IconProducer::generatedIcon()
{
    PowerManagementSettings::IconType icnType = mSettings.getIconType();
    const QString cacheStr = QString::number(icnType)
                             + QString::number(mState)
                             + QString::number(mChargePercent);
    QPixmap pixmap;
    if (!QPixmapCache::find(cacheStr, &pixmap))
    {
        QString svg;
        switch (icnType)
        {
            case PowerManagementSettings::ICON_CIRCLE:
                svg = buildIcon<PowerManagementSettings::ICON_CIRCLE>(mState, mChargePercent);
                break;
            case PowerManagementSettings::ICON_CIRCLE_ENHANCED:
                svg = buildIcon<PowerManagementSettings::ICON_CIRCLE_ENHANCED>(mState, mChargePercent);
                break;
            case PowerManagementSettings::ICON_BATTERY:
                svg = buildIcon<PowerManagementSettings::ICON_BATTERY>(mState, mChargePercent);
                break;
            case PowerManagementSettings::ICON_BATTERY_OPAQUE:
                svg = buildIcon<PowerManagementSettings::ICON_BATTERY_OPAQUE>(mState, mChargePercent);
                break;
            case PowerManagementSettings::ICON_THEME:
                Q_ASSERT(false);
                break;
        }

        // qDebug() << svg;

        // Paint the svg on a pixmap and create an icon from that.
        QSvgRenderer render{svg.toLatin1()};
        pixmap = QPixmap{QSize{256, 256}};
        pixmap.fill(QColor{0,0,0,0});
        QPainter painter{&pixmap};
        render.render(&painter);
        QPixmapCache::insert(cacheStr, pixmap);
    }

    return QIcon(pixmap);
}
