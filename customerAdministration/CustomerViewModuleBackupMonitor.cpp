#include "CustomerViewModuleBackupMonitor.h"
#include "ui_CustomerViewModuleBackupMonitor.h"

#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

#include "TcpCommands/customers/modCustomer.h"
#include "TcpCommands/customers/generateNewBackupMonitorKey.h"
#include "TcpCommands/backupMonitor/getCustomerInterfaceAddress.h"

CustomerViewModuleBackupMonitor::CustomerViewModuleBackupMonitor(QWidget *parent) :
    CustomerViewModule(parent),
    ui(new Ui::CustomerViewModuleBackupMonitor)
{
    ui->setupUi(this);

    ui->tabInstallation->setCurrentIndex(0);

    connect(ui->pushEnable, SIGNAL(clicked()), this, SLOT(enableMonitor()));
    connect(ui->pushDisable, SIGNAL(clicked()), this, SLOT(disableMonitor()));
    connect(ui->pushGenerateNewInterfaceKey, SIGNAL(clicked()), this, SLOT(generateNewInterfaceKey()));

    connect(ui->inputProxyHostname, SIGNAL(textChanged(QString)), this, SLOT(reloadClientInstallCommand()));
    connect(ui->inputProxyUsername, SIGNAL(textChanged(QString)), this, SLOT(reloadClientInstallCommand()));
    connect(ui->inputProxyPassword, SIGNAL(textChanged(QString)), this, SLOT(reloadClientInstallCommand()));
    connect(ui->pushClearProxyInputs, SIGNAL(clicked()), this, SLOT(clearProxyInputs()));

    CREATE_TCP_COMMAND(address, backupMonitor, getCustomerInterfaceAddress);
    address->startWait();
    _customerInterfaceAddress = address->getAddress();

    ui->labelMsiDownload->setTextFormat(Qt::RichText);
    ui->labelMsiDownload->setText("<a href=\"#\">"+ _customerInterfaceAddress +"</a>");
    connect(ui->labelMsiDownload, SIGNAL(linkActivated(QString)), this, SLOT(openBrowserMsiDownload()));

    reloadClientInstallCommand();
}

CustomerViewModuleBackupMonitor::~CustomerViewModuleBackupMonitor()
{
    delete ui;
}

void CustomerViewModuleBackupMonitor::openBrowserMsiDownload() {
    QDesktopServices::openUrl( QUrl(_customerInterfaceAddress) );
}

void CustomerViewModuleBackupMonitor::init(const QHash<QString, QVariant> &row) {

    // Backup- Monitor Button aktivieren / deaktivieren:
    ui->pushEnable->setEnabled( ! row["backup_enabled"].toBool() );
    ui->pushDisable->setEnabled( row["backup_enabled"].toBool() );

    ui->labelEnabled->setVisible( row["backup_enabled"].toBool() );
    ui->labelDisabled->setVisible( ! row["backup_enabled"].toBool() );

    ui->inputCustomerId->setText( row["id"].toString() );
    ui->inputInterfaceKey->setText( row["backup_interface_key"].toString() );

    _customerId = row["id"].toInt();

    // Anzahl der zu erwartenden Jobs setzen:
    ui->inputProspectiveJobsMon->setValue( row["backup_prospective_jobs_mon"].toInt() );
    ui->inputProspectiveJobsTue->setValue( row["backup_prospective_jobs_tue"].toInt() );
    ui->inputProspectiveJobsWed->setValue( row["backup_prospective_jobs_wed"].toInt() );
    ui->inputProspectiveJobsThu->setValue( row["backup_prospective_jobs_thu"].toInt() );
    ui->inputProspectiveJobsFri->setValue( row["backup_prospective_jobs_fri"].toInt() );
    ui->inputProspectiveJobsSat->setValue( row["backup_prospective_jobs_sat"].toInt() );
    ui->inputProspectiveJobsSun->setValue( row["backup_prospective_jobs_sun"].toInt() );
}

QHash<QString, QVariant> CustomerViewModuleBackupMonitor::save() {

    QHash<QString, QVariant> row;
    row["backup_prospective_jobs_mon"] = ui->inputProspectiveJobsMon->value();
    row["backup_prospective_jobs_tue"] = ui->inputProspectiveJobsTue->value();
    row["backup_prospective_jobs_wed"] = ui->inputProspectiveJobsWed->value();
    row["backup_prospective_jobs_thu"] = ui->inputProspectiveJobsThu->value();
    row["backup_prospective_jobs_fri"] = ui->inputProspectiveJobsFri->value();
    row["backup_prospective_jobs_sat"] = ui->inputProspectiveJobsSat->value();
    row["backup_prospective_jobs_sun"] = ui->inputProspectiveJobsSun->value();

    return row;
}

void CustomerViewModuleBackupMonitor::enableMonitor() {

    QHash<QString, QVariant> modColumns;
    modColumns["backup_enabled"] = QVariant(true);

    CREATE_TCP_COMMAND(mod, customers, modCustomer);
    mod->setCustomerId(_customerId);
    mod->setModColumns( modColumns );
    mod->startWait();

    // Wurde noch kein Interface- Key gesetzt, so einen setzen:
    if (ui->inputInterfaceKey->text().isEmpty()) {
        generateNewInterfaceKey();
    }

    if (mod->successfulModified()) {
        ui->pushEnable->setEnabled(false);
        ui->pushDisable->setEnabled(true);

        ui->labelEnabled->setVisible( true );
        ui->labelDisabled->setVisible( false );
    }
}

void CustomerViewModuleBackupMonitor::disableMonitor() {

    int q = QMessageBox::question(this, "Backup- Monitor deaktivieren",
                          "Sind Sie sicher, dass Sie den Backupmonitor deaktivieren wollen?",
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (q != QMessageBox::Yes) return;

    QHash<QString, QVariant> modColumns;
    modColumns["backup_enabled"] = QVariant(false);

    CREATE_TCP_COMMAND(mod, customers, modCustomer);
    mod->setCustomerId(_customerId);
    mod->setModColumns( modColumns );
    mod->startWait();

    if (mod->successfulModified()) {
        ui->pushEnable->setEnabled(true);
        ui->pushDisable->setEnabled(false);

        ui->labelEnabled->setVisible( false );
        ui->labelDisabled->setVisible( true );
    }
}

void CustomerViewModuleBackupMonitor::generateNewInterfaceKey() {

    if (!ui->inputInterfaceKey->text().isEmpty()) {
        int q = QMessageBox::question(this, "Interface- Key generieren",
                              "Soll wirklich ein neuer Key generiert werden? Der alte Key ist absofort nicht mehr gültig!",
                                      QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (q != QMessageBox::Yes) return;
    }

    CREATE_TCP_COMMAND(newKey, customers, generateNewBackupMonitorKey);
    newKey->setCustomerId(_customerId);
    newKey->startWait();

    ui->inputInterfaceKey->setText( newKey->getNewInterfaceKey() );
}

void CustomerViewModuleBackupMonitor::reloadClientInstallCommand() {

    QString additionalCommands;
    QString additionalWgetParameters;

    if (ui->inputProxyHostname->text().size() > 0) {
        additionalCommands += "export proxy_hostname=\""+ ui->inputProxyHostname->text() +"\"; ";
    }
    if (ui->inputProxyUsername->text().size() > 0) {
        additionalCommands += "export proxy_username=\""+ ui->inputProxyUsername->text() +"\"; ";
        additionalWgetParameters += "--proxy-user \""+ ui->inputProxyUsername->text() +"\"";
    }
    if (ui->inputProxyPassword->text().size() > 0) {
        additionalCommands += "export proxy_password=\""+ ui->inputProxyPassword->text() +"\"; ";
        additionalWgetParameters += "--proxy-password \""+ ui->inputProxyPassword->text() +"\"";
    }

    ui->inputClientInstallationCommand->setText("rm -f  install_siedl_backupsyncer.sh; "+ additionalCommands +" wget "+ additionalWgetParameters +" "+ _customerInterfaceAddress + "/download_sepbackupmonitor_syncer_installer.php -O install_sepbackupmonitor_syncer.sh; bash install_sepbackupmonitor_syncer.sh");
}

void CustomerViewModuleBackupMonitor::clearProxyInputs() {

    ui->inputProxyHostname->setText("");
    ui->inputProxyUsername->setText("");
    ui->inputProxyPassword->setText("");
}
