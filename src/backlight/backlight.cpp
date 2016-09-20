/*
* Copyright (c) LXQt contributors.
*
* This file is part of the LXQt project. <http://lxqt.org>
* It is distributed under the LGPL 2.1 or later license.
* Please refer to the LICENSE file for a copy of the license, and
* the AUTHORS file for copyright and authorship information.
*/

#include "backlight.h"
#include "backlightprovider.h"
#include "xrandrbacklight.h"
#include "sysbacklight.h"

#define CHECK_PROVIDER(providerclass)\
    provider = new providerclass(this);\
    if(provider->isSupported()){ \
        return provider; \
    } else \
    { \
        delete provider;\
        provider = nullptr;\
    }

BacklightProvider* Backlight::selectProvider()
{
    BacklightProvider* provider = nullptr;

    CHECK_PROVIDER(SysBacklight)
    CHECK_PROVIDER(XRandrBacklight)

    return provider;
}

#undef CHECK_PROVIDER

Backlight::Backlight(QObject *parent): QObject(parent)
{
    mProvider = selectProvider();
}


Backlight::~Backlight() {}

bool Backlight::isSupported()
{
    return mProvider != nullptr && mProvider->isSupported();
}

long Backlight::level()
{
    return isSupported() ? mProvider->level() : -1;
}

long Backlight::levelMax()
{
    return isSupported() ? mProvider->levelMax() : -1;
}

void Backlight::setLevel(long level)
{
    if(isSupported()){
        mProvider->setLevel(level);
    }
}

