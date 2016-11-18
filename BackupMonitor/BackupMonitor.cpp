#include "BackupMonitor.h"
#include "ui_BackupMonitor.h"

#include "BackupMonitor/BackupMonitorClientStatus.h"
#include "BackupMonitor/BackupMonitorListJobs.h"
#include "BackupMonitor/BackupMonitorDashBoard.h"

BackupMonitor::BackupMonitor(QWidget *parent) :
    MdiAbstractWindow(parent),
    ui(new Ui::BackupMonitor)
{
    ui->setupUi(this);

    ui->tabWidget->addTab(new BackupMonitorDashBoard(this), "Dashboard");
    ui->tabWidget->addTab(new BackupMonitorClientStatus(this), "Detailiert");

    BackupMonitorClientStatus *searchWindow = new BackupMonitorClientStatus(this, true);
    ui->tabWidget->addTab(searchWindow, "Durchsuchen");
}

BackupMonitor::~BackupMonitor()
{
    delete ui;
}
