/*
 * Copyright (c) 2015 Dmitry Kontsevoy
 *
 * This file is part of the LXQt project. <http://lxqt.org>
 * It is distributed under the LGPL 2.1 or later license.
 * Please refer to the LICENSE file for a copy of the license, and
 * the AUTHORS file for copyright and authorship information.
 */

#ifndef BACKLIGHTWATCHER_H
#define BACKLIGHTWATCHER_H

#include <QObject>
#include <backlight/backlight.h>
#include <Solid/Battery>

#include "../config/powermanagementsettings.h"

class BacklightWatcher: public QObject
{
    Q_OBJECT

public:
    explicit BacklightWatcher(QObject *parent = nullptr);
    virtual ~BacklightWatcher();

private slots:
    void updateBacklightLevel();

private:
    QList<Solid::Battery*> mBatteries;

    PowerManagementSettings mSettings;
    Backlight mBacklight;
};

#endif

