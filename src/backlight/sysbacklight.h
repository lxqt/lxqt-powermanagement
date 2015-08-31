/*
* Copyright (c) LXQt contributors.
*
* This file is part of the LXQt project. <http://lxqt.org>
* It is distributed under the LGPL 2.1 or later license.
* Please refer to the LICENSE file for a copy of the license, and
* the AUTHORS file for copyright and authorship information.
*/

#ifndef SYSBACKLIGHT_H
#define SYSBACKLIGHT_H

#include "backlightprovider.h"
#define PREFIX "/sys/class/backlight/"

/*
 * To work this class require write permission on /sys/class/backlight/.../brightness
 * It may be automatically set by simple udev rule.
 */
class SysBacklight: public BacklightProvider
{
    Q_OBJECT

public:
    explicit SysBacklight(QObject *parent = nullptr);
    virtual ~SysBacklight();
    
    bool isSupported() const;
    long level() const;
    long levelMax() const;
    void setLevel(long level);

private:
    QString mDirname;
};

#endif
