#include "BackupMonitorExport.h"
#include "ui_BackupMonitorExport.h"

#include "TcpCommands/backupMonitor/getCustomers.h"
#include "TcpCommands/backupMonitor/getJobs.h"

#include "BackupMonitor/BackupMonitorExportStatusDialog.h"
#include "BackupMonitor/BackupMonitorExportThread.h"

#include <QFileDialog>
#include <QMessageBox>

BackupMonitorExport::BackupMonitorExport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BackupMonitorExport)
{
    ui->setupUi(this);

    // Alle verfügbaren Backup- Kunden auslesen:
    CREATE_TCP_COMMAND(customerList, backupMonitor, getCustomers);
    customerList->startWait();
    QList< QPair<quint32, QString> > customers = customerList->getCustomerList();

    QPair<quint32, QString> customer;
    foreach(customer, customers) {

        QListWidgetItem *item = new QListWidgetItem;
        item->setText(customer.second);
        item->setData(Qt::UserRole + 1, customer.first);
        ui->listCustomers->addItem(item);
    }

    // Von Datum ist ein Jahr zurück, Bis Datum ist das heutige Datum:
    ui->inputDateFrom->setDate( QDate::currentDate().addYears(-1) );
    ui->inputDateTo->setDate( QDate::currentDate() );

    connect(ui->pushChooseFolder, SIGNAL(clicked()), this, SLOT(chooseFolder()));
    connect(ui->pushCreate, SIGNAL(clicked()), this, SLOT(create()));
}

BackupMonitorExport::~BackupMonitorExport()
{
    delete ui;
}

void BackupMonitorExport::create() {

    int customersSelected = ui->listCustomers->selectedItems().count();
    if (customersSelected == 0) {
        QMessageBox::critical(this, "Fehler", "Es wurden keine Kunden ausgewählt!");
        return;
    }

    QDate fromDate = ui->inputDateFrom->date();
    QDate toDate = ui->inputDateTo->date();
    if (fromDate > toDate) {
        QMessageBox::critical(this, "Fehler", "'Von' Datum darf nicht größer sein als 'bis' Datum.");
        return;
    }

    QDir destDir(ui->inputDir->text());
    if (ui->inputDir->text().isEmpty() || !destDir.isReadable()) {
        QMessageBox::critical(this, "Fehler", "Zielverzeichnis kann nicht gelesen werden.");
        return;
    }


    QList< QPair<int, QString > > customersList;
    foreach(QListWidgetItem *item ,ui->listCustomers->selectedItems()) {
        QPair<int, QString > data;
        data.first = item->data(Qt::UserRole + 1).toInt();
        data.second = item->text();
        customersList.append(data);
    }

    BackupMonitorExportStatusDialog dialogStatus;
    dialogStatus.thread()->setData(customersList, ui->inputDateFrom->date(), ui->inputDateTo->date(), ui->inputDir->text());
    dialogStatus.exec();
}

void BackupMonitorExport::chooseFolder() {

    QString dir = QFileDialog::getExistingDirectory(this, trUtf8("Wähle ein Verzeichnis aus"),
                                                    "",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    ui->inputDir->setText(dir);
}

void BackupMonitorExport::exportFinished() {

    QMessageBox::information(this, "Export abgeschlossen", "Der Export wurde erfolgreich abgeschlossen!");
}
