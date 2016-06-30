/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXDE-Qt - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright (C) 2012  Alec Moskvin <alecm@gmx.com>
 * Copyright (c) 2016  Luís Pereira <luis.artur.pereira@gmail.com>
 * Copyright (c) 2012 The Chromium Authors. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#include "x11helper.h"

#include <QtGui/QGuiApplication>
#include <qpa/qplatformnativeinterface.h>

#include <QX11Info>
#include <X11/extensions/scrnsaver.h>


static bool GetProperty(XID window, const std::string& property_name, long max_length,
                 Atom* type, int* format, unsigned long* num_items,
                 unsigned char** property);


static bool GetIntArrayProperty(XID window,
                         const std::string& property_name,
                         std::vector<int>* value);


static bool GetProperty(XID window, const std::string& property_name, long max_length,
                 Atom* type, int* format, unsigned long* num_items,
                 unsigned char** property)
{
    Atom property_atom =  XInternAtom(X11Helper::display(), property_name.c_str(), false);
    unsigned long remaining_bytes = 0;
    return XGetWindowProperty(QX11Info::display(),
                              window,
                              property_atom,
                              0,          // offset into property data to read
                              max_length, // max length to get
                              False,      // deleted
                              AnyPropertyType,
                              type,
                              format,
                              num_items,
                              &remaining_bytes,
                              property);
}

static bool GetIntArrayProperty(XID window,
                         const std::string& property_name,
                         std::vector<int>* value)
{
    Atom type = None;
    int format = 0;  // size in bits of each item in 'property'
    unsigned long num_items = 0;
    unsigned char* properties = NULL;

    int result = GetProperty(window, property_name,
                           (~0L), // (all of them)
                           &type, &format, &num_items, &properties);
    XScopedPtr<unsigned char> scoped_properties(properties);
    if (result != Success)
        return false;

    if (format != 32)
        return false;

    long* int_properties = reinterpret_cast<long*>(properties);
    value->clear();
    for (unsigned long i = 0; i < num_items; ++i)
    {
        value->push_back(static_cast<int>(int_properties[i]));
    }
    return true;
}



Display* X11Helper::display()
{
    QPlatformNativeInterface *native = qApp->platformNativeInterface();
    void* display = native->nativeResourceForWindow("display", 0);
    return reinterpret_cast<Display*>(display);
}

xcb_connection_t* X11Helper::connection()
{
    QPlatformNativeInterface *native = qApp->platformNativeInterface();
    void* connection = native->nativeResourceForWindow("connection", 0);
    return reinterpret_cast<xcb_connection_t*>(connection);
}

bool X11Helper::isScreenSaverLocked()
{
    XScreenSaverInfo *info = 0;
    info = XScreenSaverAllocInfo();

    XScreenSaverQueryInfo(display(), DefaultRootWindow(display()), info);
    const int state = info->state;
    XFree(info);
    if (state == ScreenSaverOn)
        return true;

    // Ironically, xscreensaver does not conform to the XScreenSaver protocol, so
    // info.state == ScreenSaverOff or info.state == ScreenSaverDisabled does not
    // necessarily mean that a screensaver is not active, so add a special check
    // for xscreensaver.
    XAtom lock_atom = XInternAtom(display(), "LOCK", false);
    std::vector<int> atom_properties;
    if (GetIntArrayProperty(DefaultRootWindow(display()), "_SCREENSAVER_STATUS", &atom_properties) &&
        atom_properties.size() > 0)
    {
        if (atom_properties[0] == static_cast<int>(lock_atom))
            return true;
    }

    return false;
}
