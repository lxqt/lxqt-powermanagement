/*
* Copyright (c) LXQt contributors.
*
* This file is part of the LXQt project. <http://lxqt.org>
* It is distributed under the LGPL 2.1 or later license.
* Please refer to the LICENSE file for a copy of the license, and
* the AUTHORS file for copyright and authorship information.
*/

#ifndef BACKLIGHTPROVIDER_H
#define BACKLIGHTPROVIDER_H

#include <QObject>

class BacklightProvider: public QObject
{
    Q_OBJECT

public:
    explicit BacklightProvider(QObject *parent = nullptr);
    virtual ~BacklightProvider();

    virtual bool isSupported() const = 0;
    virtual long level() const = 0;
    virtual long levelMax() const = 0;
    virtual void setLevel(long level) = 0;
};

#endif
