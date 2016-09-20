/*
* Copyright (c) LXQt contributors.
*
* This file is part of the LXQt project. <http://lxqt.org>
* It is distributed under the LGPL 2.1 or later license.
* Please refer to the LICENSE file for a copy of the license, and
* the AUTHORS file for copyright and authorship information.
*/

#include <QX11Info>
#include <QScopedPointer>
#include "xrandrbacklight.h"

XRandrBacklight::XRandrBacklight(QObject *parent): BacklightProvider(parent)
{
    auto connection = QX11Info::connection();
    
    if(!connection){
        qWarning("no xcb connection");
        return;
    }

    QScopedPointer<xcb_randr_query_version_reply_t> versionReply(xcb_randr_query_version_reply(connection,
                xcb_randr_query_version(connection, 1, 2),
                nullptr));

    if(!versionReply) {
        qWarning("RandR Query version returned null");
        return;
    }

    if(versionReply->major_version < 1 || (versionReply->major_version == 1 && versionReply->minor_version < 2)) {
        qWarning("RandR version %d.%d too old", versionReply->major_version, versionReply->minor_version);
        return;
    }

    const char* atomName = "Backlight";
    QScopedPointer<xcb_intern_atom_reply_t> backlightReply(xcb_intern_atom_reply(connection,
                xcb_intern_atom(connection, 1, strlen(atomName), atomName),
                nullptr));

    if(!backlightReply){
        qWarning("Intern Atom for Backlight returned null");
        return;
    }

    m_backlight = backlightReply->atom;

    if(m_backlight == XCB_ATOM_NONE){
        qWarning("No outputs have backlight property");
        return;
    }

    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(xcb_get_setup(QX11Info::connection()));

    if (!iter.rem) {
        qWarning("XCB Screen Roots Iterator rem was null");
        return;
    }

    xcb_screen_t *screen = iter.data;

    xcb_window_t root = screen->root;

    m_resources.reset(xcb_randr_get_screen_resources_current_reply(QX11Info::connection(),
                xcb_randr_get_screen_resources_current(QX11Info::connection(), root),
                nullptr));

    if (!m_resources) {
        qWarning("RANDR Get Screen Resources returned null");
        return;
    }

}

XRandrBacklight::~XRandrBacklight()
{
}

bool XRandrBacklight::isSupported() const
{
    if (!m_resources) {
        return false;
    }

    auto *outputs = xcb_randr_get_screen_resources_current_outputs(m_resources.data());

    for (int i = 0; i < m_resources->num_outputs; ++i) {
        if (backlight_get(outputs[i]) != -1) {
            return true;
        }
    }
    return false;
}

long XRandrBacklight::level() const
{
    if (!m_resources) {
        return 0;
    }

    auto *outputs = xcb_randr_get_screen_resources_current_outputs(m_resources.data());

    for (int i = 0; i < m_resources->num_outputs; ++i) {
        auto output = outputs[i];

        long cur, min, max;
        if (backlight_get_with_range(output, cur, min, max)) {
            // FIXME for now just return the first output's value
            return cur - min;
        }

    }

    return 0;
}

long XRandrBacklight::levelMax() const
{
    if (!m_resources) {
        return 0;
    }

    auto *outputs = xcb_randr_get_screen_resources_current_outputs(m_resources.data());
    for (int i = 0; i < m_resources->num_outputs; ++i) {
        auto output = outputs[i];

        long cur, min, max;
        if (backlight_get_with_range(output, cur, min, max)) {
            // FIXME for now just return the first output's value
            return max - min;
        }
    }

    return 0;
}

void XRandrBacklight::setLevel(long value)
{
    if (!m_resources) {
        return;
    }

    auto *outputs = xcb_randr_get_screen_resources_current_outputs(m_resources.data());
    for (int i = 0; i < m_resources->num_outputs; ++i) {
        auto output = outputs[i];

        long cur, min, max;
        if (backlight_get_with_range(output, cur, min, max)) {
            // FIXME for now just set the first output's value
            backlight_set(output, min + value);
        }
    }

    free(xcb_get_input_focus_reply(QX11Info::connection(), xcb_get_input_focus(QX11Info::connection()), nullptr)); // sync
}

bool XRandrBacklight::backlight_get_with_range(xcb_randr_output_t output, long &value, long &min, long &max) const {
    long cur = backlight_get(output);
    if (cur == -1) {
        return false;
    }

    QScopedPointer<xcb_randr_query_output_property_reply_t> propertyReply(xcb_randr_query_output_property_reply(QX11Info::connection(),
                xcb_randr_query_output_property(QX11Info::connection(), output, m_backlight)
                , nullptr));

    if (!propertyReply) {
        return -1;
    }

    if (propertyReply->range && xcb_randr_query_output_property_valid_values_length(propertyReply.data()) == 2) {
        int32_t *values = xcb_randr_query_output_property_valid_values(propertyReply.data());
        value = cur;
        min = values[0];
        max = values[1];
        return true;
    }

    return false;
}

long XRandrBacklight::backlight_get(xcb_randr_output_t output) const
{
    QScopedPointer<xcb_randr_get_output_property_reply_t> propertyReply;
    long value;

    if (m_backlight != XCB_ATOM_NONE) {
        propertyReply.reset(xcb_randr_get_output_property_reply(QX11Info::connection(),
                    xcb_randr_get_output_property(QX11Info::connection(), output, m_backlight, XCB_ATOM_NONE, 0, 4, 0, 0)
                    , nullptr));

        if (!propertyReply) {
            return -1;
        }
    }

    if (!propertyReply || propertyReply->type != XCB_ATOM_INTEGER || propertyReply->num_items != 1 || propertyReply->format != 32) {
        value = -1;
    } else {
        value = *(reinterpret_cast<long *>(xcb_randr_get_output_property_data(propertyReply.data())));
    }
    return value;
}

void XRandrBacklight::backlight_set(xcb_randr_output_t output, long value)
{
    xcb_randr_change_output_property(QX11Info::connection(), output, m_backlight, XCB_ATOM_INTEGER,
            32, XCB_PROP_MODE_REPLACE,
            1, reinterpret_cast<unsigned char *>(&value));
}
