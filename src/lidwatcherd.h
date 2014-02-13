/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXDE-Qt - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2012 Razor team
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
#ifndef LXQTAUTOSUSPENDD_H
#define LXQTAUTOSUSPENDD_H

#include <QObject>
#include <QTime>
#include <lxqt/lxqtnotification.h>
#include <lxqt/lxqtsettings.h>
#include <lxqt/lxqtpower.h>
#include "lid.h"

class LidWatcherd : public QObject
{
    Q_OBJECT
public:
    explicit LidWatcherd(QObject *parent = 0);
    virtual ~LidWatcherd();

private slots:
    void lidChanged(bool closed);

private:
    void doAction(int action);
    int action();
    bool externalMonitorPlugged();

    Lid mLid;
    LxQt::Power mLxQtPower;
    LxQt::Settings mSettings;

};

#endif // LXQTAUTOSUSPENDD_H
