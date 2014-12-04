#ifndef ICONNAMEFINDER_H
#define ICONNAMEFINDER_H
#include <QString>
#include <QMap>
#include <QObject>

class ThemeIconFinder : private QObject
{
public:
    ThemeIconFinder();
    QString iconName(float chargeLevel, bool discharging);
    void themeChanged();

private:
    QMap<float, QString> mLevelNameMapCharging;
    QMap<float, QString> mLevelNameMapDischarging;

};

#endif // ICONNAMEFINDER_H
