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
    QIcon icon(float chargeLevel, bool discharging);

private:
    QIcon themedIcon(float chargeLevel, bool discharging);
    QIcon builtInIcon(float chargelevel, bool discharging);
    void updateLevelNameMaps();

    PowerManagementSettings mSettings;
    QString mLastThemeName;

    QMap<float, QString> mLevelNameMapCharging;
    QMap<float, QString> mLevelNameMapDischarging;



};

#endif // ICONPRODUCER_H
