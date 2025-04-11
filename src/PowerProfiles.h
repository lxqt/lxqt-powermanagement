/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXQt - a lightweight, Qt based, desktop toolset
 * https://lxqt.org
 *
 * Copyright: 2025~ LXQt team
 * Authors:
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

#pragma once

#include <QObject>
#include "dbus_types.h"

class QAction;
class QMenu;
class QActionGroup;

namespace LXQt
{
    class DBusPropAsyncGetter;

    class PowerProfiles : public QObject
    {
        Q_OBJECT
    public:
        enum PPProperties
        {
            PPP_ActiveProfile = 0
                , PPP_Profiles
                , PPP_Count
        };
    public:
        static PowerProfiles & instance();

    public:
        PowerProfiles(QObject * parent = nullptr);
        ~PowerProfiles();

        bool isValid() const;
        const QString & activeProfile() const;
        void setActiveProfile(const QString & value);
        QAction * menuAction();

    Q_SIGNALS:
        void needPropertyFetch(const QString & name);
        void needPropertyPush(const QString & name, const QVariant & value);
        void propertyFetched(const QString & name, const QVariant & value);
        void propertyPushed(const QString & name);

    protected:
        void onFetched(const QString name, const QVariant value);
        void reassembleMenu(const QListOfQVariantMap & profiles);

    private:
        static const QString msDBusPPService;
        static const QString msDBusPPPath;
        static const QString msDBusPPInterface;
        static const QString msDBusPPProperties[PPP_Count];

    private:
        // Note: we don't want to rely on the qdbusxml2cpp generated interface as it
        // generates fetching/setting properties as DBus synch calls and we must not be blocking
        // on the main/ui thread.
        //std::unique_ptr<org::freedesktop::UPower::PowerProfiles> mDBusProfiles;
        std::unique_ptr<QThread> mThread;
        std::unique_ptr<DBusPropAsyncGetter> mPropGetter;

        std::unique_ptr<QAction> mMenuAction;
        std::unique_ptr<QMenu> mMenu;
        std::unique_ptr<QActionGroup> mActions;
        QString mActiveProfile;
    };
}
