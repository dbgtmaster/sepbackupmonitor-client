#include "BackupMonitorManageGroupsNew.h"
#include "ui_BackupMonitorManageGroupsNew.h"

#include "TcpCommands/backupMonitor/customerGroupMod.h"

#include <QMessageBox>

BackupMonitorManageGroupsNew::BackupMonitorManageGroupsNew(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BackupMonitorManageGroupsNew)
{
    ui->setupUi(this);

    _created = false;

    connect(ui->pushCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->pushCreate, SIGNAL(clicked()), this, SLOT(create()));
}

BackupMonitorManageGroupsNew::~BackupMonitorManageGroupsNew()
{
    delete ui;
}

void BackupMonitorManageGroupsNew::create() {

    QHash<QString, QVariant> cols;
    cols["name"] = ui->widget->getName();
    cols["description"] = ui->widget->getDescription();
    cols["auto_expand"] = ui->widget->isAutoExpand();

    CREATE_TCP_COMMAND(mod, backupMonitor, customerGroupMod);
    mod->setAction(TcpCommand_backupMonitor_customerGroupMod::CREATE);
    mod->setCols(cols);
    mod->startWait();

    if (!mod->isSuccessful()) {
        // Es ist ein Fehler beim Speichern aufgetreten, Benutzer informieren:
        QMessageBox::critical(this, "Fehler", mod->errorMessage());
        return;
    }

    /**
      * Gruppe wurde erfolgreich angelegt..
      */
    _created = true;
    close();
}
