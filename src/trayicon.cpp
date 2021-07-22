/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXQt - a lightweight, Qt based, desktop toolset
 * https://lxqt.org
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
#include <QApplication>
#include <QProcess>
#include <QMessageBox>
#include <QToolTip>
#include <QHelpEvent>
#include <QPixmapCache>
#include <QPainter>
#include <QtSvg/QSvgRenderer>
#include <Solid/Battery>
#include <Solid/Device>
#include <XdgIcon>

#include "trayicon.h"
#include "batteryhelper.h"
#include "../config/powermanagementsettings.h"

#include <LXQt/Globals>
#include <LXQt/Notification>

TrayIcon::TrayIcon(Solid::Battery *battery, QObject *parent)
    : QSystemTrayIcon(parent),
    mBattery(battery),
    mIconProducer(battery),
    mContextMenu(),
    mHasPauseEmblem(false)
{
    connect(mBattery, &Solid::Battery::chargePercentChanged, this, &TrayIcon::updateTooltip);
    connect(mBattery, &Solid::Battery::chargeStateChanged, this, &TrayIcon::updateTooltip);
    updateTooltip();

    connect(&mIconProducer, &IconProducer::iconChanged, this, &TrayIcon::iconChanged);
    iconChanged();

    connect(this, &TrayIcon::activated, this, &TrayIcon::onActivated);

    mContextMenu.addAction(XdgIcon::fromTheme(QStringLiteral("configure")), tr("Configure"),
                           this, &TrayIcon::onConfigureTriggered);

    // pause actions
    mPauseActions = new QActionGroup(this);
    mPauseActions->setExclusionPolicy(QActionGroup::ExclusionPolicy::ExclusiveOptional);
    connect(mPauseActions, &QActionGroup::triggered, this, &TrayIcon::onPauseTriggered);

    QAction *a = new QAction(tr("30 minutes"), mPauseActions);
    a->setCheckable(true);
    a->setData(PAUSE::Half);

    a = new QAction(tr("1 hour"), mPauseActions);
    a->setCheckable(true);
    a->setData(PAUSE::One);

    a = new QAction(tr("2 hours"), mPauseActions);
    a->setCheckable(true);
    a->setData(PAUSE::Two);

    a = new QAction(tr("3 hours"), mPauseActions);
    a->setCheckable(true);
    a->setData(PAUSE::Three);

    a = new QAction(tr("4 hours"), mPauseActions);
    a->setCheckable(true);
    a->setData(PAUSE::Four);

    QMenu *pauseMenu = mContextMenu.addMenu(XdgIcon::fromTheme(QStringLiteral("media-playback-pause")),
                                            tr("Pause idleness checks"));
    pauseMenu->addActions(mPauseActions->actions());

    mContextMenu.addSeparator();

    mContextMenu.addAction(XdgIcon::fromTheme(QStringLiteral("help-about")), tr("About"),
                           this, &TrayIcon::onAboutTriggered);
    mContextMenu.addAction(XdgIcon::fromTheme(QStringLiteral("edit-delete")), tr("Disable icon"),
                           this, &TrayIcon::onDisableIconTriggered);
    setContextMenu(&mContextMenu);
}

TrayIcon::~TrayIcon()
{
}

void TrayIcon::iconChanged()
{
    mHasPauseEmblem = PowerManagementSettings().isIdlenessWatcherPaused();
    setIcon(mHasPauseEmblem ? emblemizedIcon() : mIconProducer.mIcon);
}

QIcon TrayIcon::emblemizedIcon()
{
    static const QString pause_emblem = QL1S(
        "<svg version='1.1' xmlns='http://www.w3.org/2000/svg' viewBox='0 0 16 16'>\n"
        "<path style='fill:#fff;fill-opacity:0.65' d='M 0,0 H 16 V 16 H 0 Z'/>\n"
        "<path d='M 4,3 V 13 H 7 V 3 Z m 5,0 v 10 h 3 V 3 Z'/>\n"
        "</svg>"
    );

    const QSize icnSize(200, 200); // NOTE: QSystemTrayIcon::geometry() gives an empty rectangle
    QPixmap icnPix = mIconProducer.mIcon.pixmap(icnSize);
    const qreal pixelRatio = icnPix.devicePixelRatio();
    const QSize icnPixSize((QSizeF(icnSize) * pixelRatio).toSize());

    // first get the emblem pixmap
    const QString cacheStr = QString::number(icnPixSize.width())
                             + QString::number(icnPixSize.height());
    QPixmap emblemPix;
    if (!QPixmapCache::find(cacheStr, &emblemPix))
    {
        QSvgRenderer render(pause_emblem.toLocal8Bit());
        emblemPix = QPixmap(icnPixSize / 2);
        emblemPix.fill(Qt::transparent);
        emblemPix.setDevicePixelRatio(pixelRatio);
        QPainter p(&emblemPix);
        render.render(&p);
        QPixmapCache::insert(cacheStr, emblemPix);
    }

    // add the emblem to the icon
    QPixmap pix = QPixmap(icnPixSize);
    pix.fill(Qt::transparent);
    pix.setDevicePixelRatio(pixelRatio);
    QPainter painter(&pix);
    painter.drawPixmap(0, 0, icnPix);
    const int offset = icnSize.width() / 2;
    painter.drawPixmap(offset, offset, emblemPix); // bottom right

    return QIcon(pix);
}

void TrayIcon::updateTooltip()
{
    QString tooltip = BatteryHelper::stateToString(mBattery->chargeState());
    tooltip += QString::fromLatin1(" (%1 %)").arg(mBattery->chargePercent());
    setToolTip(tooltip);
}

void TrayIcon::onConfigureTriggered()
{
    QProcess::startDetached(QL1S("lxqt-config-powermanagement"), QStringList());
}

void TrayIcon::onPauseTriggered(QAction *action)
{
    emit pauseChanged(!action->isChecked() ? PAUSE::None
                                           : static_cast<PAUSE>(action->data().toInt()));
}

void TrayIcon::onAboutTriggered()
{
    QMessageBox::about(nullptr,
                       tr("About"),
                       tr( "<p>"
                           "  <b>LXQt Power Management</b><br/>"
                           "  - Power Management for the LXQt Desktop Environment"
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
    auto notification = new LXQt::Notification{tr("LXQt Power Management info"), nullptr};
    notification->setBody(tr("The LXQt Power Management tray icon can be (re)enabled in <i>lxqt-config-powermanagement</i>"));
    notification->setIcon(QSL("preferences-system-power-management"));
    notification->setActions({tr("Configure now")});
    notification->setUrgencyHint(LXQt::Notification::UrgencyLow);
    connect(notification, &LXQt::Notification::actionActivated, [notification] { notification->close(); QProcess::startDetached(QL1S("lxqt-config-powermanagement"), QStringList()); });
    connect(notification, &LXQt::Notification::notificationClosed, notification, &QObject::deleteLater);
    notification->update();

    PowerManagementSettings().setShowIcon(false);
}

void TrayIcon::onActivated(QSystemTrayIcon::ActivationReason reason)
{
    //qDebug() << "onActivated" << reason;
    if (Trigger == reason) emit toggleShowInfo();
}

void TrayIcon::setPause(PAUSE duration)
{
    // add/remove the pause emblem and correct the checked action if needed
    QAction *checked = mPauseActions->checkedAction();
    if (duration == PAUSE::None)
    {
        PowerManagementSettings().setIdlenessWatcherPaused(false);
        if (mHasPauseEmblem)
            iconChanged(); // removes the pause emblem
        if (checked != nullptr)
            checked->setChecked(false);
    }
    else
    {
        PowerManagementSettings().setIdlenessWatcherPaused(true);
        if (!mHasPauseEmblem)
            iconChanged(); // adds the pause emblem
        if (checked == nullptr || checked->data().toInt() != duration)
        {
            const auto actions = mPauseActions->actions();
            for (const auto &a : actions)
            {
                if (a->data().toInt() == duration)
                {
                    a->setChecked(true);
                    break;
                }
            }
        }
    }
}

// static
int TrayIcon::getPauseInterval(PAUSE duration)
{
    // multiplied by 30 minutes.
    return duration * 1800 * 1000;
}

