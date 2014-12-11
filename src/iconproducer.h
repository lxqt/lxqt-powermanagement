#ifndef ICONPRODUCER_H
#define ICONPRODUCER_H
#include <QString>
#include <QMap>
#include <QObject>
#include "../config/powermanagementsettings.h"
#include "battery.h"

class IconProducer : public QObject
{
    Q_OBJECT

public:
    IconProducer(Battery* battery, QObject *parent = 0);
    IconProducer(QObject *parent = 0);

    QIcon mIcon;
    QString mIconName;

signals:
    void iconChanged();

public slots:
    void update(float newChargeLevel, Battery::State newState);

private slots:
    void themeChanged();
    void update();

private:

    QIcon &circleIcon();
    QIcon buildCircleIcon(Battery::State state, double chargeLevel);


    float mChargeLevel;
    Battery::State mState;

    PowerManagementSettings mSettings;

    QMap<float, QString> mLevelNameMapCharging;
    QMap<float, QString> mLevelNameMapDischarging;

};

#endif // ICONPRODUCER_H
