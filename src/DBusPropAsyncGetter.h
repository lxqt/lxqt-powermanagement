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
#include <QDBusConnection>

namespace LXQt
{
    class DBusPropAsyncGetter : public QObject
    {
        Q_OBJECT
    public:
        DBusPropAsyncGetter(const QString & service, const QString & path, const QString & interface, const QDBusConnection & conn);

    public Q_SLOTS:
        // Note: interthread signal/slot communication - parameters by values
        void fetch(const QString name);
        void push(const QString name, const QVariant value);

    public Q_SLOTS:
        void onPropertiesChanged(const QString & interfaceName, const QVariantMap & changedProperties, const QStringList & invalidatedProperties);

    Q_SIGNALS:
        // Note: interthread signal/slot communication - parameters by values
        void fetched(const QString name, const QVariant value);
        void pushed(const QString name);
        void serviceDisappeared();

    private:
        const QString mService;
        const QString mPath;
        const QString mInterface;
        QDBusConnection mConn;
    };
}
