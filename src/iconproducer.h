#ifndef ICONPRODUCER_H
#define ICONPRODUCER_H
#include <QString>
#include <QMap>
#include <QObject>
#include "../config/powermanagementsettings.h"
#include <Solid/Battery>

class IconProducer : public QObject
{
    Q_OBJECT

public:
    IconProducer(Solid::Battery* battery, QObject *parent = 0);
    IconProducer(QObject *parent = 0);

    QIcon mIcon;
    QString mIconName;

signals:
    void iconChanged();

public slots:
    void update(int newChargeLevel, Solid::Battery::ChargeState newState);

private slots:
    void themeChanged();
    void update();

private:

    QIcon &circleIcon();
    QIcon buildCircleIcon(Solid::Battery::ChargeState state, int chargeLevel);


    float mChargeLevel;
    Solid::Battery::ChargeState mState;

    PowerManagementSettings mSettings;

    QMap<float, QString> mLevelNameMapCharging;
    QMap<float, QString> mLevelNameMapDischarging;

};

#endif // ICONPRODUCER_H
