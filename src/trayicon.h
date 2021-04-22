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

#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <Solid/Battery>

#include "iconproducer.h"
#include "../config/powermanagementsettings.h"

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

public:
    enum PAUSE {
        None  = 0,
        Half  = 1,
        One   = 2, // two halves
        Two   = 4,
        Three = 6,
        Four  = 8
    };

    TrayIcon(Solid::Battery *battery, QObject *parent = nullptr);
    ~TrayIcon() override;

    static int getPauseInterval(PAUSE duration);

    void setPause(PAUSE duration);

signals:
    void toggleShowInfo();
    void pauseChanged(PAUSE duration);

public slots:
    void iconChanged();
    void updateTooltip();

private slots:
    void onConfigureTriggered();
    void onPauseTriggered(QAction *action);
    void onAboutTriggered();
    void onDisableIconTriggered();
    void onActivated(QSystemTrayIcon::ActivationReason reason);

private:
    QIcon emblemizedIcon();

    Solid::Battery *mBattery;
    IconProducer mIconProducer;
    QMenu mContextMenu;
    QActionGroup *mPauseActions;
    bool mHasPauseEmblem;
};

#endif // TRAYICON_H
