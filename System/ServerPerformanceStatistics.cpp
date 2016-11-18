#include "ServerPerformanceStatistics.h"
#include "ui_ServerPerformanceStatistics.h"

#include <QTimer>

#include "TcpCommands/system/serverStatistics.h"

ServerPerformanceStatistics::ServerPerformanceStatistics(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerPerformanceStatistics)
{
    ui->setupUi(this);

    QTimer::singleShot(0, this, SLOT(reloadStats()));

}

ServerPerformanceStatistics::~ServerPerformanceStatistics()
{
    delete ui;
}

void ServerPerformanceStatistics::reloadStats() {

    CREATE_TCP_COMMAND(serverStats, system, serverStatistics);
    serverStats->startWait();

    QHash<QString, QVariant> serverData = serverStats->getServerData();

    ui->labelDatabasePoolMin->setText( QString::number( serverData["databasepool_min"].toInt() ) );
    ui->labelDatabasePoolMax->setText( QString::number( serverData["databasepool_max"].toInt() ) );
    ui->labelDatabasePoolFree->setText( QString::number( serverData["databasepool_free"].toInt() ) );
    ui->labelDatabasePoolManaged->setText( QString::number( serverData["databasepool_total_managed"].toInt() ) );

    ui->labelBackupStateOfDayCustomersEntries->setText( QString::number( serverData["cache_backup_state_of_day_customers_entries"].toInt() ) );
    ui->labelBackupStateOfDayCustomersMaxEntries ->setText( QString::number( serverData["cache_backup_state_of_day_customers_max_entries"].toInt() ) );
}
