#ifndef ICONPRODUCER_H
#define ICONPRODUCER_H
#include <QString>
#include <QMap>
#include <QObject>
#include "../config/powermanagementsettings.h"

class IconProducer
{
public:
    IconProducer();
    void update(float chargeLevel);
    QIcon icon(float chargeLevel, bool discharging);
    QString iconName(float chargeLevel, bool discharging);

private:
    QIcon themedIcon(float chargeLevel, bool discharging);
    QIcon builtInIcon(float chargelevel, bool discharging);
    void updateLevelNameMaps();

    PowerManagementSettings mSettings;
    QString mLastThemeName;
    float mChargeLevel;


    QMap<float, QString> mLevelNameMapCharging;
    QMap<float, QString> mLevelNameMapDischarging;

    QIcon mChargingIcon;
    QIcon mDischargingIcon;

};

#endif // ICONPRODUCER_H
