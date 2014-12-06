#include "batteryinfodialog.h"
#include "ui_batteryinfodialog.h"

#include <QFormLayout>
#include <QTabWidget>
#include <QDebug>

BatteryInfoDialog::BatteryInfoDialog(QList<Battery*> batteries, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BatteryInfoDialog)
{
    ui->setupUi(this);

    setWindowTitle(tr("Battery Info"));

    if (batteries.size() == 1)
    {
        BatteryInfoFrame *batteryInfoFrame = new BatteryInfoFrame(batteries[0]);
        ui->verticalLayout->insertWidget(0, batteryInfoFrame);
    }
    else
    {
        QTabWidget *tabWidget = new QTabWidget(this);
        ui->verticalLayout->insertWidget(0, tabWidget);
        foreach (Battery *battery, batteries)
        {
            BatteryInfoFrame *batteryInfoFrame = new BatteryInfoFrame(battery);
            tabWidget->addTab(batteryInfoFrame, "BAT");
        }
    }

}

BatteryInfoDialog::~BatteryInfoDialog()
{
    delete ui;
}


void BatteryInfoDialog::toggleShow()
{
    qDebug() << "toggleShow";
    isVisible() ? hide() : show();
}
