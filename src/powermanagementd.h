/* 
 * File:   powermanagementd.h
 * Author: Christian Surlykke
 *
 * Created on 9. februar 2014, 16:15
 */

#ifndef POWERMANAGEMENTD_H
#define	POWERMANAGEMENTD_H

#include <lxqt/lxqtsettings.h>
#include "../config/powermanagementsettings.h"

class BatteryWatcher;
class LidWatcher;
class IdlenessWatcher;

class PowerManagementd : public QObject
{
    Q_OBJECT

public:
    PowerManagementd();
    virtual ~PowerManagementd();

private slots:
    void settingsChanged();

private:
    BatteryWatcher* mBatterywatcherd;
    LidWatcher* mLidwatcherd;
    IdlenessWatcher* mIdlenesswatcherd;

    PowerManagementSettings mSettings;
};

#endif	/* POWERMANAGEMENTD_H */

