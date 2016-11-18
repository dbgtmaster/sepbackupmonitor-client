#include "BackupMonitorDashBoard.h"
#include "ui_BackupMonitorDashBoard.h"

#include "BackupMonitor/BackupMonitorGlobals.h"
#include "BackupMonitor/BackupMonitorDashBoardNumericItem.h"
#include "BackupMonitor/BackupMonitorClientStatus.h"

#include "TcpCommands/customers/search.h"
#include "TcpCommands/backupMonitor/getBackupStateOfDay.h"

#include <QDateTime>

BackupMonitorDashBoard::BackupMonitorDashBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BackupMonitorDashBoard)
{
    ui->setupUi(this);

    connect(ui->pushRefresh, SIGNAL(clicked()), this, SLOT(_refreshDashBoard()));

    _refreshDashBoard();
}

BackupMonitorDashBoard::~BackupMonitorDashBoard()
{
    delete ui;
}

void BackupMonitorDashBoard::_refreshDashBoard() {

    ui->labelRefreshDate->setText( "Anzeige vom: " + QDateTime::currentDateTime().toString() );

    /**
      * Sync State:
      */
    ui->tableSyncAndConfigErrors->clear();
    ui->tableSyncAndConfigErrors->clearContents();
    ui->tableSyncAndConfigErrors->setColumnCount(2);
    ui->tableSyncAndConfigErrors->setRowCount(0);   // Remove currect rows
    ui->tableSyncAndConfigErrors->setHorizontalHeaderLabels( QStringList() << "Kunde" << "Fehler" );
    ui->tableSyncAndConfigErrors->setColumnWidth(0, 350);
    ui->tableSyncAndConfigErrors->setColumnWidth(1, 900);
    ui->tableFailedCustomers->setEditTriggers(QAbstractItemView::NoEditTriggers);

    /**
     * Kundenstatus:
     */
    ui->tableFailedCustomers->clear();
    ui->tableFailedCustomers->setColumnCount(4);
    ui->tableFailedCustomers->setRowCount(0);   // Remove currect rows
    ui->tableFailedCustomers->setHorizontalHeaderLabels( QStringList() << "Kunde" << "Fehlerhaft seit" << "Fehler Durchschnitt" << "Jobs noch laufend");
    ui->tableFailedCustomers->setColumnWidth(0, 350);
    ui->tableFailedCustomers->setColumnWidth(1, 230);
    ui->tableFailedCustomers->setColumnWidth(2, 130);
    ui->tableFailedCustomers->setColumnWidth(3, 130);
    ui->tableFailedCustomers->setEditTriggers(QAbstractItemView::NoEditTriggers);

    CREATE_TCP_COMMAND(getCustomers, customers, search);
    getCustomers->setReturnColumns( QStringList() << "backup_prospective_jobs_mon" << "backup_prospective_jobs_tue"
                                    << "backup_prospective_jobs_wed" << "backup_prospective_jobs_thu"
                                    << "backup_prospective_jobs_fri" << "backup_prospective_jobs_sat"
                                    << "backup_prospective_jobs_sun" << "customer_name" << "id"
                                    << "backup_last_hello");
    getCustomers->setSearchFilter("1");
    getCustomers->setFilterColumns( QStringList() << "backup_enabled" );
    getCustomers->startWait();

    QList<QString> daysOfWeek;
    daysOfWeek << "sun" << "mon" << "tue" << "wed" << "thu" << "fri" << "sat" << "sun";

    int tableSyncAndConfigErrorsRow = 0;
    int tableCustomerFailedRow = 0;
    while( getCustomers->hasNextCustomer() ) {

        QHash<QString, QVariant> customer = getCustomers->fetchNextCustomer();

        int lastSyncSeconds = QDateTime::currentDateTime().toTime_t() - customer["backup_last_hello"].toInt();
        bool lastSyncTimeValid = !(QDateTime::fromTime_t(customer["backup_last_hello"].toInt()) < QDateTime::fromTime_t( QDateTime::currentDateTime().toTime_t() - SECONDS_SYNC_INVALID ) );

        if (!lastSyncTimeValid) {

            ui->tableSyncAndConfigErrors->insertRow(tableSyncAndConfigErrorsRow);
            ui->tableSyncAndConfigErrors->setItem(tableSyncAndConfigErrorsRow, 0, new QTableWidgetItem(QIcon(":/red.png"), customer["customer_name"].toString()));
            ui->tableSyncAndConfigErrors->setItem(tableSyncAndConfigErrorsRow, 1, new QTableWidgetItem("Letzter Sync vor " + QString::number( lastSyncSeconds / 3600  ) + " Stunden") );
            tableSyncAndConfigErrorsRow++;
        }

        // Wurden Backuptage beim Kunden definiert??
        if (customer["backup_prospective_jobs_mon"].toInt() < 1 &&
                customer["backup_prospective_jobs_tue"].toInt() < 1 &&
                customer["backup_prospective_jobs_wed"].toInt() < 1 &&
                customer["backup_prospective_jobs_thu"].toInt() < 1 &&
                customer["backup_prospective_jobs_fri"].toInt() < 1 &&
                customer["backup_prospective_jobs_sat"].toInt() < 1 &&
                customer["backup_prospective_jobs_sun"].toInt() < 1) {

            ui->tableSyncAndConfigErrors->insertRow(tableSyncAndConfigErrorsRow);
            ui->tableSyncAndConfigErrors->setItem(tableSyncAndConfigErrorsRow, 0, new QTableWidgetItem(QIcon(":/red.png"), customer["customer_name"].toString()));
            ui->tableSyncAndConfigErrors->setItem(tableSyncAndConfigErrorsRow, 1, new QTableWidgetItem("Keine Backuptage konfiguriert.") );
            tableSyncAndConfigErrorsRow++;
        }
        else {

            int daysBackward = 20;     // Wie viele Tage vorraus angezeigt werden sollen.

            /**
             * Wir ermitteln nun eine Liste mit den Datums, welche Rückblickend angezeigt werden sollen...
             */
            QList< QDate > dates;
            QDate tmpDate = QDate::currentDate();
            for (int i = 1; i <= daysBackward; i++) {

                tmpDate = tmpDate.addDays(-1);

                // Ist dieser Tag ein Backup- Tag?
                if (customer["backup_prospective_jobs_" + daysOfWeek[tmpDate.dayOfWeek()]].toInt() < 1) {
                    i--;
                }
                else {
                    // Dieser Tag war ein Backup- Tag
                    dates.append(tmpDate);
                }
            }

            /**
             * Und nun Fragen wir die Statuse aller Backups in unserer Liste ab:
             */
            CREATE_TCP_COMMAND(getStateOfDayBackward, backupMonitor, getBackupStateOfDay);
            getStateOfDayBackward->setCustomerId(customer["id"].toInt());
            getStateOfDayBackward->setDates(dates);
            getStateOfDayBackward->startWait();

            int errorDays = 0;      // Wie viele Tage Rückblickend sind fehlerhalft
            int errorJobs = 0;
            bool allJobsFinished = true;
            bool allDaysFailed = true;
            while (getStateOfDayBackward->hasNextState()) {

                QHash<QByteArray, QVariant> state = getStateOfDayBackward->getNextState();

                if (state["error"].toInt() > 0 || state["aborted"].toInt() > 0) {

                    // Falls am letzten Backuptag noch jobs laufen, so anzeigen:
                    if (errorDays == 0 && state["running"].toInt() > 0) {
                        allJobsFinished = false;
                    }

                    errorDays++;
                    errorJobs += state["error"].toInt() + state["aborted"].toInt();
                }
                else if (state["totalWithoutMainJobs"].toInt() < customer["backup_prospective_jobs_" + daysOfWeek[state["date"].toDate().dayOfWeek()]].toInt()) {
                    errorDays++;
                }
                else if (errorDays == 0 && state["running"].toInt() > 0) {
                    allJobsFinished = false;
                }
                else {
                    allDaysFailed = false;
                    break;
                }
            }

            if (errorDays > 0) {

                ui->tableFailedCustomers->insertRow(tableCustomerFailedRow);

                ui->tableFailedCustomers->setItem(tableCustomerFailedRow, 0, new QTableWidgetItem(QIcon(":/red.png"), customer["customer_name"].toString()));
                if (allDaysFailed) {
                    ui->tableFailedCustomers->setItem(tableCustomerFailedRow, 1, new BackupMonitorDashBoardNumericItem( QString::number(errorDays) + " Backuptage oder mehr" ) );
                }
                else {
                    ui->tableFailedCustomers->setItem(tableCustomerFailedRow, 1, new BackupMonitorDashBoardNumericItem( QString::number(errorDays) + " Backuptage" ) );
                }
                ui->tableFailedCustomers->setItem(tableCustomerFailedRow, 2, new BackupMonitorDashBoardNumericItem( QString::number(errorJobs / errorDays) + " Jobs" ) );
                if (allJobsFinished) {
                    ui->tableFailedCustomers->setItem(tableCustomerFailedRow, 3, new BackupMonitorDashBoardNumericItem("nein") );
                }
                else {
                    ui->tableFailedCustomers->setItem(tableCustomerFailedRow, 3, new BackupMonitorDashBoardNumericItem("ja") );
                }

                tableCustomerFailedRow++;
            }
        }
    }

    ui->tableFailedCustomers->setSortingEnabled(true);
    ui->tableFailedCustomers->sortByColumn(1, Qt::DescendingOrder);

    ui->tableSyncAndConfigErrors->setSortingEnabled(true);
    ui->tableSyncAndConfigErrors->sortByColumn(0, Qt::AscendingOrder);
}
