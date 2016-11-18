#include "BackupMonitorExportStatusDialog.h"
#include "ui_BackupMonitorExportStatusDialog.h"

#include <QDebug>
#include <QDateTime>
#include <QMessageBox>

#include "BackupMonitor/BackupMonitorExportThread.h"

BackupMonitorExportStatusDialog::BackupMonitorExportStatusDialog(QWidget *parent) :
    QDialog(parent), _thread(0), _errors(false), _finished(false),
    ui(new Ui::BackupMonitorExportStatusDialog)
{
    ui->setupUi(this);

    connect(ui->pushCancel, SIGNAL(clicked()), this, SLOT(cancel()));

    /**
      * THREAD initialisieren:
      **/
    _thread = new BackupMonitorExportThread(this);
    connect(_thread, SIGNAL(state(int,int)), this, SLOT(status(int,int)));
    connect(_thread, SIGNAL(stateMessage(QString)), this, SLOT(statusMessage(QString)));
    connect(_thread, SIGNAL(errorMessage(QString)), this, SLOT(errorMessage(QString)));
    connect(_thread, SIGNAL(finished()), this, SLOT(exportFinished()));
    connect(_thread, SIGNAL(finished()), _thread, SLOT(deleteLater()));
}

int BackupMonitorExportStatusDialog::exec() {

    // Thread starten:
    _thread->start();

    // Dialog anzeigen:
    return QDialog::exec();
}

BackupMonitorExportStatusDialog::~BackupMonitorExportStatusDialog()
{
    delete ui;
}

void BackupMonitorExportStatusDialog::status(int protocolDaysFinished, int protocolDaysTotal) {

    ui->progressBarTotal->setMaximum(protocolDaysTotal);
    ui->progressBarTotal->setValue(protocolDaysFinished);
    ui->inputStatusProtocolsTotal->setText( QString("%1/%2").arg(QString::number(protocolDaysFinished), QString::number(protocolDaysTotal)) );
}

void BackupMonitorExportStatusDialog::exportFinished() {

    if (_errors) {
        QMessageBox::critical(this, "Export fehlerhaft", "Beim Export sind ein oder mehrere Fehler aufgetreten! Bitte Log überprüfen!");
    }
    else {
        QMessageBox::information(this, "Export abgeschlossen", "Export wurde abgeschlossen!");
    }
    _thread = 0;
    _finished = true;
}

void BackupMonitorExportStatusDialog::statusMessage(const QString &message) {
    ui->inputStatus->appendPlainText( QString("%1: %2").arg(QDateTime::currentDateTime().toString(), message));
}

void BackupMonitorExportStatusDialog::errorMessage(const QString &message) {
    statusMessage("ERROR: " + message);
    _errors = true;
}

void BackupMonitorExportStatusDialog::cancel() {

    if (_finished) return;

    // Wurde bereits auf abbrechen gedrückt?
    if (!ui->pushCancel->isEnabled()) return;

    errorMessage("Export wird vom Benutzer abgebrochen, Bitte warten...");

    ui->pushCancel->setEnabled(false);
    _thread->stop();
}

void BackupMonitorExportStatusDialog::closeEvent(QCloseEvent *event) {

    if (_thread == 0 || _thread->isFinished()) {
        event->accept();
        return;
    }
    event->ignore();

    // Thread sagen, dieser solle sich beenden...
    cancel();
}
