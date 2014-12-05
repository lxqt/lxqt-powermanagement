#include "batteryinfodialog.h"
#include "ui_batteryinfodialog.h"
#include "batteryinfo.h"

#include <QVBoxLayout>
#include <QTabWidget>
BatteryInfoDialog::BatteryInfoDialog(QList<Battery*> batteries, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BatteryInfoDialog)
{
    ui->setupUi(this);

    setWindowTitle(tr("Battery Info"));

    if (batteries.size() == 1)
    {
        BatteryInfo *batteryInfo = new BatteryInfo(batteries[0]);
        ui->verticalLayout->insertWidget(0, batteryInfo);
    }
    else
    {
        QTabWidget *tabWidget = new QTabWidget(this);
        ui->verticalLayout->insertWidget(0, tabWidget);
        foreach (Battery *battery, batteries)
        {
            BatteryInfo *batteryInfo = new BatteryInfo(battery);
            tabWidget->addTab(batteryInfo, "BAT");
        }
    }

}

BatteryInfoDialog::~BatteryInfoDialog()
{
    delete ui;
}
