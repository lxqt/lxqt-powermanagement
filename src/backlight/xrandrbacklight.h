/*
* Copyright (c) LXQt contributors.
*
* This file is part of the LXQt project. <http://lxqt.org>
* It is distributed under the LGPL 2.1 or later license.
* Please refer to the LICENSE file for a copy of the license, and
* the AUTHORS file for copyright and authorship information.
*/

#ifndef XRANDRBACKLIGHT_H
#define XRANDRBACKLIGHT_H

#include <xcb/xcb.h>
#include <xcb/randr.h>
#include "backlight.h"

class XRandrBacklight: public BacklightProvider
{
    Q_OBJECT

public:
    explicit XRandrBacklight (QObject *parent = nullptr);
    virtual ~XRandrBacklight ();

    bool isSupported() const;
    long level() const;
    long levelMax() const;
    void setLevel(long level);

private:
    xcb_atom_t m_backlight = XCB_ATOM_NONE;
    QScopedPointer<xcb_randr_get_screen_resources_current_reply_t> m_resources;

    bool backlight_get_with_range(xcb_randr_output_t output, long &value, long &min, long &max) const;
    long backlight_get(xcb_randr_output_t output) const;
    void backlight_set(xcb_randr_output_t output, long value);
};

#endif
