/*
* Copyright (c) LXQt contributors.
*
* This file is part of the LXQt project. <http://lxqt.org>
* It is distributed under the LGPL 2.1 or later license.
* Please refer to the LICENSE file for a copy of the license, and
* the AUTHORS file for copyright and authorship information.
*/

#ifndef BACKLIGHT_H
#define BACKLIGHT_H

#include <QObject>
#include "backlightprovider.h"

class Backlight: QObject 
{
    Q_OBJECT

public:

    explicit Backlight(QObject *parent=nullptr);
    virtual ~Backlight();

    bool isSupported();
    long level();
    long levelMax();
    void setLevel(long level);

private:
    BacklightProvider* selectProvider();

    BacklightProvider* mProvider = nullptr;
};

#endif
