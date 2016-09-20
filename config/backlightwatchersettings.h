/*
 * Copyright (c) 2015 Dmitry Kontsevoy
 *
 * This file is part of the LXQt project. <http://lxqt.org>
 * It is distributed under the LGPL 2.1 or later license.
 * Please refer to the LICENSE file for a copy of the license, and
 * the AUTHORS file for copyright and authorship information.
 */

#ifndef BACKLIGHTWATCHERSETTINGS_H
#define BACKLIGHTWATCHERSETTINGS_H

#include <QWidget>

#include "../config/powermanagementsettings.h"

namespace Ui {
    class BacklightWatcherSettings;
}

class BacklightWatcherSettings : public QWidget
{
    Q_OBJECT

public:
    explicit BacklightWatcherSettings(QWidget *parent = 0);
    ~BacklightWatcherSettings();

public slots:
    void loadSettings();

private slots:
    void saveSettings();

private:
    PowerManagementSettings mSettings;
    Ui::BacklightWatcherSettings *mUi;
};

#endif // POWERLOWSETTINGS_H

