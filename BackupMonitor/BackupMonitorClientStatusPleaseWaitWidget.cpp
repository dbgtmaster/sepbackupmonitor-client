#include "BackupMonitorClientStatusPleaseWaitWidget.h"
#include "ui_BackupMonitorClientStatusPleaseWaitWidget.h"

BackupMonitorClientStatusPleaseWaitWidget::BackupMonitorClientStatusPleaseWaitWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BackupMonitorClientStatusPleaseWaitWidget)
{
    ui->setupUi(this);
}

BackupMonitorClientStatusPleaseWaitWidget::~BackupMonitorClientStatusPleaseWaitWidget()
{
    delete ui;
}


void BackupMonitorClientStatusPleaseWaitWidget::reloadState(QString t, int now, int total) {

    ui->labelText->setText(t);
    if (total == 0) {
        ui->progressBar->setHidden(true);
    }
    else {
        ui->progressBar->setMaximum(total);
        ui->progressBar->setValue(now);
        ui->progressBar->setHidden(false);
    }
}
