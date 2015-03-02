#ifndef BATTERYINFODIALOG_H
#define BATTERYINFODIALOG_H

#include "batteryinfoframe.h"

#include <QDialog>
#include <QList>
#include <Solid/Battery>

namespace Ui {
class BatteryInfoDialog;
}

class BatteryInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BatteryInfoDialog(QList<Solid::Battery*> batteries, QWidget *parent = 0);
    ~BatteryInfoDialog();

public slots:
    void toggleShow();

private:
    Ui::BatteryInfoDialog *ui;
};

#endif // BATTERYINFODIALOG_H
