/*
* Copyright (c) LXQt contributors.
*
* This file is part of the LXQt project. <http://lxqt.org>
* It is distributed under the LGPL 2.1 or later license.
* Please refer to the LICENSE file for a copy of the license, and
* the AUTHORS file for copyright and authorship information.
*/

#include <QFile>
#include <QByteArray>
#include <QDir>
#include <QTextStream>
#include <QDebug>
#include "sysbacklight.h"

SysBacklight::SysBacklight(QObject *parent):
    BacklightProvider(parent)
{
    QDir dir(PREFIX);
    dir.setFilter(QDir::AllDirs | QDir::NoDot | QDir::NoDotDot | QDir::NoDotAndDotDot | QDir::Readable);
    dir.setSorting(QDir::Name | QDir::Reversed); // Reverse is needed to priorize acpi_video1 over 0

    QStringList entries = dir.entryList();

    if (entries.isEmpty()) {
        qWarning() << "No backlight interfaces found";
        return;
    }

    QFile file;
    QFileInfo fileinfo;
    QByteArray buffer;
    QStringList firmware, platform, raw;

    for(auto interface: entries)
    {
        // Check permissions
        fileinfo.setFile(PREFIX + interface + "/brightness");
        if(!fileinfo.isWritable() || !fileinfo.isReadable()) {
            continue;
        }
        fileinfo.setFile(PREFIX + interface + "/max_brightness");
        if(!fileinfo.isReadable()) {
            continue;
        }

        file.setFileName(PREFIX + interface + "/type");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            continue;
        }

        buffer = file.readLine().trimmed();
        if (buffer == "firmware") {
            firmware.append(interface);
        } else if(buffer == "platform") {
            platform.append(interface);
        } else if (buffer == "raw") {
            raw.append(interface);
        } else {
            qWarning() << "Interface type not handled" << buffer;
        }

        file.close();
    }

    if (!firmware.isEmpty()) {
        mDirname = PREFIX + firmware.first();
        return;
    }
 
    if (!platform.isEmpty()) {
        mDirname = PREFIX + platform.first();
        return;
    }
 
    if (!raw.isEmpty()) {
        mDirname = PREFIX + raw.first();
        return;
    }
}

SysBacklight::~SysBacklight()
{}

bool SysBacklight::isSupported() const
{
    return mDirname != "";
}

long SysBacklight::level() const
{
    QFile file(mDirname + "/brightness");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Can't open file" << file.fileName();
        return 0;
    }

    long brightness;
    QTextStream stream(&file);
    stream >> brightness;

    file.close();

    return brightness;
}

long SysBacklight::levelMax() const
{
    QFile file(mDirname + "/max_brightness");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Can't open file" << file.fileName();
        return 0;
    }

    long brightness;
    QTextStream stream(&file);
    stream >> brightness;

    file.close();

    return brightness;
}

void SysBacklight::setLevel(long level)
{
    QFile file(mDirname + "/brightness");

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Can't open file" << file.fileName();
        return ;
    }

    int result = file.write(QByteArray::number((qlonglong)level));

    file.close();

    if(result == -1)
    {
        qWarning() << "writing file failed" << file.fileName();
    }
}

