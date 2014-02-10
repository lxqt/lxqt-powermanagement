/* 
 * File:   powermanagementd.h
 * Author: Christian Surlykke
 *
 * Created on 9. februar 2014, 16:15
 */

#ifndef POWERMANAGEMENTD_H
#define	POWERMANAGEMENTD_H

#include <lxqt/lxqtsettings.h>


class BatteryWatcherd;
class LidWatcherd;
class IdlenessWatcherd;

class PowerManagementd : public QObject
{
    Q_OBJECT

public:
    PowerManagementd();
    virtual ~PowerManagementd();

private slots:
    void settingsChanged();

private:
    template <class Watcher> void startStopWatcher(Watcher*& watcher, bool enabled);

    LxQt::Settings mSettings;

    BatteryWatcherd* mBatterywatcherd;
    LidWatcherd* mLidwatcherd;
    IdlenessWatcherd* mIdlenesswatcherd;
};

#endif	/* POWERMANAGEMENTD_H */

