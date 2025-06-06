/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXQt - a lightweight, Qt based, desktop toolset
 * https://lxqt.org
 *
 * Copyright: 2011 Razor team
 *            2025~ LXQt team
 * Authors:
 *   Christian Surlykke <christian@surlykke.dk>
 *   Palo Kisa <palo.kisa@gmail.com>
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

#include <QProcess>
#include <QActionGroup>
#include <QMessageBox>
#include <QPixmapCache>
#include <QPainter>
#include <QtSvg/QSvgRenderer>
#include <QStringBuilder>

#include "trayicon.h"
#include "../config/powermanagementsettings.h"

#include <LXQt/Globals>
#include <LXQt/Notification>
#include "PowerProfiles.h"
#include <QTimer>

QList<TrayIcon *> TrayIcon::msInstances;
std::unique_ptr<QMenu> TrayIcon::msContextMenu{nullptr};
std::unique_ptr<QTimer> TrayIcon::msPauseTimer{nullptr};
std::unique_ptr<QActionGroup> TrayIcon::msPauseActions{nullptr};

TrayIcon::TrayIcon(QObject *parent)
    : QSystemTrayIcon(parent),
    mBaseIcon(QIcon::fromTheme(QL1S("preferences-system-power-management"))),
    mHasPauseEmblem(false)
{

    iconChanged();

    connect(this, &TrayIcon::activated, this, &TrayIcon::onActivated);

    if (!msPauseTimer)
    {
        msPauseTimer.reset(new QTimer);
        msPauseTimer->setSingleShot(true);
        msPauseTimer->setTimerType(Qt::VeryCoarseTimer);
        connect(msPauseTimer.get(), &QTimer::timeout, [] { onPauseTimeout(); });
    }
    if (!msContextMenu) {
        msContextMenu.reset(new QMenu);
        connect(msContextMenu->addAction(QIcon::fromTheme(QStringLiteral("configure")), tr("Configure")), &QAction::triggered, [] { onConfigureTriggered(); });

        // pause actions
        msPauseActions.reset(new QActionGroup{nullptr});
        msPauseActions->setExclusionPolicy(QActionGroup::ExclusionPolicy::ExclusiveOptional);
        connect(msPauseActions.get(), &QActionGroup::triggered, [] (QAction * action) { onPauseTriggered(action); });

        QAction *a = new QAction(tr("30 minutes"), msPauseActions.get());
        a->setCheckable(true);
        a->setData(PAUSE::Half);

        a = new QAction(tr("1 hour"), msPauseActions.get());
        a->setCheckable(true);
        a->setData(PAUSE::One);

        a = new QAction(tr("2 hours"), msPauseActions.get());
        a->setCheckable(true);
        a->setData(PAUSE::Two);

        a = new QAction(tr("3 hours"), msPauseActions.get());
        a->setCheckable(true);
        a->setData(PAUSE::Three);

        a = new QAction(tr("4 hours"), msPauseActions.get());
        a->setCheckable(true);
        a->setData(PAUSE::Four);

        QMenu *pauseMenu = msContextMenu->addMenu(QIcon::fromTheme(QStringLiteral("media-playback-pause")),
                tr("Pause idleness checks"));
        pauseMenu->addActions(msPauseActions->actions());

        // power-profiles actions
        msContextMenu->addAction(LXQt::PowerProfiles::instance().menuAction());

        msContextMenu->addSeparator();

        connect(msContextMenu->addAction(QIcon::fromTheme(QStringLiteral("help-about")), tr("About")), &QAction::triggered, [] { TrayIcon::onAboutTriggered(); });
        connect(msContextMenu->addAction(QIcon::fromTheme(QStringLiteral("edit-delete")), tr("Disable icon")), &QAction::triggered, [] { TrayIcon::onDisableIconTriggered(); });
    }
    setContextMenu(msContextMenu.get());
    msInstances.push_back(this);
}

TrayIcon::~TrayIcon()
{
    msInstances.removeOne(this);
    if (msInstances.empty())
    {
        msPauseTimer.reset(nullptr);
        msPauseActions.reset(nullptr);
        msContextMenu.reset(nullptr);
    }
}

/*virtual*/ const QIcon & TrayIcon::getIcon() const
{
    return mBaseIcon;
}

void TrayIcon::iconChanged()
{
    mHasPauseEmblem = PowerManagementSettings().isIdlenessWatcherPaused();
    setIcon(mHasPauseEmblem ? emblemizedIcon() : getIcon());
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
    QPixmap icnPix = getIcon().pixmap(icnSize);
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

void TrayIcon::onPauseTimeout()
{
    for (const auto &trayIcon : std::as_const(msInstances))
        trayIcon->setPause(TrayIcon::PAUSE::None);
}

void TrayIcon::onConfigureTriggered()
{
    QProcess::startDetached(QL1S("lxqt-config-powermanagement"), QStringList());
}

void TrayIcon::onPauseTriggered(QAction *action)
{
    const PAUSE duration = !action->isChecked() ? None : static_cast<PAUSE>(action->data().toInt());
    if (duration == None)
    {
        onPauseTimeout();
        msPauseTimer->stop();
    }
    else
    {
        for (const auto &trayIcon : std::as_const(msInstances))
            trayIcon->setPause(duration);
        msPauseTimer->start(getPauseInterval(duration));
    }
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
                           "  Copyright &copy; 2012-2025"
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
    QAction *checked = msPauseActions->checkedAction();
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
            const auto actions = msPauseActions->actions();
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

