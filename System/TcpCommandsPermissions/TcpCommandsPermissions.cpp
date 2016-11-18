#include "TcpCommandsPermissions.h"
#include "ui_TcpCommandsPermissions.h"

#include <TcpCommands/system/getAllTcpCommands.h>
#include <TcpCommands/system/getAllSystemGroups.h>

#include <QHash>
#include <QStandardItem>
#include <QStandardItemModel>

#include <QSharedPointer>

#include "System/TcpCommandsPermissions/TcpCommandsItemDelegate.h"

#include "TcpCommands/system/saveTcpCommandsPermissions.h"

TcpCommandsPermissions::TcpCommandsPermissions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TcpCommandsPermissions)
{
    ui->setupUi(this);

    connect(ui->pushCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->pushSave, SIGNAL(clicked()), this, SLOT(save()));

    ui->pushSave->setEnabled(false);

    ui->tcpActionTreeView->setModel(&_tcpCommandsItemModel);

    /**
      * Alle Systemgruppen auslesen und dem Model übergeben:
      */
    CREATE_TCP_COMMAND(systemGroups, system, getAllSystemGroups);
    systemGroups->startWait();

    QSharedPointer< QMap<quint32, QString> > sysGroups( new QMap<quint32, QString> );
    while (systemGroups->hasNext()) {
        _tcpCommandsItemModel.addSystemGroup(systemGroups->groupId(), systemGroups->groupName());
    }

    /**
      * Alle Tcp- Befehle auslesen und ins Model speichern:
      */
    CREATE_TCP_COMMAND(tcpCommands, system, getAllTcpCommands);
    tcpCommands->startWait();

    while (tcpCommands->hasNext()) {

        // Der Tcp- Befehl:
        TcpCommandsItemModel::tcpCommand command (
                    tcpCommands->commandId(),
                    tcpCommands->commandGroup(),
                    tcpCommands->commandName(),
                    tcpCommands->systemGroupId()
                    );

        // Und hinzufügen:
        _tcpCommandsItemModel.addCommand( command );
    }

    /**
      * Delegate:
      */
    TcpCommandsItemDelegate* delegate = new TcpCommandsItemDelegate;
    ui->tcpActionTreeView->setItemDelegate(delegate);
    connect(delegate, SIGNAL(sysGroupValueChanged()), this, SLOT(systemGroupChanged()) );

    ui->tcpActionTreeView->setColumnWidth(0, 270);

    ui->tcpActionTreeView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

TcpCommandsPermissions::~TcpCommandsPermissions()
{
    delete ui;
}

void TcpCommandsPermissions::systemGroupChanged() {
    ui->pushSave->setEnabled(true);
}

void TcpCommandsPermissions::save() {

    // Aktuell bearbeitendes Item beenden:
    _tcpCommandsItemModel.submit();

    CREATE_TCP_COMMAND(savePermissions, system, saveTcpCommandsPermissions);

    // Liste holen mit allen vorgenommenen Änderungen (QList<[Befehls ID], [new SysGroupId]):
    savePermissions->setPermissions( &_tcpCommandsItemModel.getAllChangedSysGroups() );
    savePermissions->startWait();

    close();
}
