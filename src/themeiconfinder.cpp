#include "themeiconfinder.h"
#include <LXQt/Settings>
#include <XdgIcon>
#include <QDebug>

ThemeIconFinder::ThemeIconFinder()
{
    themeChanged();
}

void ThemeIconFinder::themeChanged()
{
    mLevelNameMapCharging.clear();
    mLevelNameMapDischarging.clear();

    QString themeName = QIcon::themeName(); // FIXME - doesn't work
    qDebug() << "themeName:" << themeName;
    if (themeName == "oxygen")
    {
        qDebug() << "Using oxygen battery icons";
        mLevelNameMapCharging[20] = "battery-charging-low";       // Means:
        mLevelNameMapDischarging[20] = "battery-low";             // Use 'battery-low'/'battery-charging-low' for levels up to 20
        mLevelNameMapCharging[40] = "battery-charging-caution";   //
        mLevelNameMapDischarging[40] = "battery-caution";         //  -  'battery-caution'/'battery-charging-caution' for levels between 20 and 40
        mLevelNameMapCharging[60] = "battery-charging-040";       //
        mLevelNameMapDischarging[60] = "battery-040";             // etc..
        mLevelNameMapCharging[80] = "battery-charging-060";
        mLevelNameMapDischarging[80] = "battery-060";
        mLevelNameMapCharging[99] = "battery-charging-080";
        mLevelNameMapDischarging[99] = "battery-080";
        mLevelNameMapCharging[100] = "battery-charging";
        mLevelNameMapDischarging[100] = "battery-100";

    }
    else if (themeName == "Awoken")
    {
        qDebug() << "Using Awoken icons";
        mLevelNameMapCharging[20] = "battery-000-charging";
        mLevelNameMapDischarging[20] = "battery-000";
        mLevelNameMapCharging[40] = "battery-020-charging";
        mLevelNameMapDischarging[40] = "battery-020";
        mLevelNameMapCharging[60] = "battery-040-charging";
        mLevelNameMapDischarging[60] = "battery-040";
        mLevelNameMapCharging[80] = "battery-060-charging";
        mLevelNameMapDischarging[80] = "battery-060";
        mLevelNameMapCharging[99] = "battery-080-charging";
        mLevelNameMapDischarging[99] = "battery-080";
        mLevelNameMapCharging[100] = "battery-100-charging";
        mLevelNameMapDischarging[100] = "battery-100";
   }
    else // Free desktop scheme
    {
        qDebug() << "Using freedesktop icons";
        mLevelNameMapCharging[1] = "battery-empty";
        mLevelNameMapDischarging[1] = "battery-empty";
        mLevelNameMapCharging[20] = "battery-caution-charging";
        mLevelNameMapDischarging[20] = "battery-caution";
        mLevelNameMapCharging[40] = "battery-low-charging";
        mLevelNameMapDischarging[40] = "battery-low";
        mLevelNameMapCharging[60] = "battery-good-charging";
        mLevelNameMapDischarging[60] = "battery-good";
        mLevelNameMapCharging[100] = "battery-full-charging";
        mLevelNameMapDischarging[100] = "battery-full";

    }


}


QString ThemeIconFinder::iconName(float chargeLevel, bool discharging)
{
    QMap<float, QString> *levelNameMap =  (discharging ? &mLevelNameMapDischarging : &mLevelNameMapCharging);
    foreach (float level, levelNameMap->keys())
    {
        if (level >= chargeLevel)
            return levelNameMap->value(level);
    }

    return QString();
}
