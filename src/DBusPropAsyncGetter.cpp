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

#include "DBusPropAsyncGetter.h"
#include <QDBusMessage>
#include <QDBusPendingCallWatcher>
#include <QDBusPendingReply>
#include <QDBusServiceWatcher>

using namespace Qt::Literals::StringLiterals;

namespace LXQt
{
    DBusPropAsyncGetter::DBusPropAsyncGetter(const QString & service, const QString & path, const QString & interface, const QDBusConnection & conn)
        : mService{service}
        , mPath{path}
        , mInterface{interface}
        , mConn{conn}
    {
        if (!mConn.connect(mService
                    , mPath
                    , "org.freedesktop.DBus.Properties"_L1
                    , "PropertiesChanged"_L1
                    , this
                    , SLOT(onPropertiesChanged(QString, QVariantMap, QStringList))
                    )
           )
            qDebug().noquote().nospace() << "Could not connect to org.freedesktop.DBus.Properties.PropertiesChanged()(" << mService << ',' << mPath << ')';

        connect(new QDBusServiceWatcher{mService, mConn, QDBusServiceWatcher::WatchForUnregistration, this}
                , &QDBusServiceWatcher::serviceUnregistered
                , this
                , [this] { Q_EMIT serviceDisappeared(); }
               );
    }

    void DBusPropAsyncGetter::fetch(const QString name)
    {
        QDBusMessage msg = QDBusMessage::createMethodCall(mService, mPath, "org.freedesktop.DBus.Properties"_L1, "Get"_L1);
        msg << mInterface << name;
        connect(new QDBusPendingCallWatcher{mConn.asyncCall(msg), this}
            , &QDBusPendingCallWatcher::finished
            , this
            , [this, name] (QDBusPendingCallWatcher * call) {
                QDBusPendingReply<QVariant> reply = *call;
                if (reply.isError())
                    qDebug().noquote().nospace() << "Error on DBus request(" << mService << ',' << mPath << ',' << name << "): " << reply.error();
                Q_EMIT fetched(name, reply.value());
                call->deleteLater();
            }
        );
    }

    void DBusPropAsyncGetter::push(const QString name, const QVariant value)
    {
        QDBusMessage msg = QDBusMessage::createMethodCall(mService, mPath, "org.freedesktop.DBus.Properties"_L1, "Set"_L1);
        msg << mInterface << name << value;
        connect(new QDBusPendingCallWatcher{mConn.asyncCall(msg), this}
            , &QDBusPendingCallWatcher::finished
            , this
            , [this, name] (QDBusPendingCallWatcher * call) {
                QDBusPendingReply<> reply = *call;
                if (reply.isError())
                    qDebug().noquote().nospace() << "Error on DBus request(" << mService << ',' << mPath << ',' << mInterface << ',' << name << "): " << reply.error();
                else
                    Q_EMIT pushed(name);
                call->deleteLater();
            }
        );
    }

    void DBusPropAsyncGetter::onPropertiesChanged(const QString & /*interfaceName*/, const QVariantMap & changedProperties, const QStringList & invalidatedProperties)
    {
        for (const auto & [key, value] : changedProperties.asKeyValueRange())
            Q_EMIT fetched(key, value);
        for (const auto & key : invalidatedProperties)
            Q_EMIT fetched(key, {});
    }
}
