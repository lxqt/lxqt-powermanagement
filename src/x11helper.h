/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXDE-Qt - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright (C) 2013  Alec Moskvin <alecm@gmx.com>
 * Copyright (c) 2016 Luís Pereira <luis.artur.pereira@gmail.com>
 * Copyright (c) 2013 The Chromium Authors. All rights reserved.
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

#ifndef X11HELPER_H
#define X11HELPER_H

#include <memory>

#include <xcb/xcb.h>

// Avoid polluting everything with X11/Xlib.h:
typedef struct _XDisplay Display;

typedef unsigned long XAtom;
typedef unsigned long XID;

extern "C" {
int XFree(void*);
}

template <class T, class R, R (*F)(T*)>
struct XObjectDeleter {
  inline void operator()(void* ptr) const { F(static_cast<T*>(ptr)); }
};

template <class T, class D = XObjectDeleter<void, int, XFree>>
using XScopedPtr = std::unique_ptr<T, D>;

/**
 * @brief The X11Helper class is class to get the X11 Display or XCB connection
 *
 * It's intended to be used as a wrapper/replacement for QX11Info, which is removed in Qt5.
 */
class X11Helper
{
public:
    /**
     * @brief display Returns the X11 display
     * @return
     */
    static Display* display();

    /**
     * @brief connection Returns the XCB connection
     * @return
     */
    static xcb_connection_t* connection();

    static bool isScreenSaverLocked();
};

#endif // X11HELPER_H
