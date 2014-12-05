#ifndef BATTERYINFODIALOG_H
#define BATTERYINFODIALOG_H

#include "battery.h"

#include <QDialog>
#include <QList>

namespace Ui {
class BatteryInfoDialog;
}

class BatteryInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BatteryInfoDialog(QList<Battery*> batteries, QWidget *parent = 0);
    ~BatteryInfoDialog();

private:
    Ui::BatteryInfoDialog *ui;
};

#endif // BATTERYINFODIALOG_H
