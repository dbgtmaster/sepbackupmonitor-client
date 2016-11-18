#include "TcpCommandsItemModel.h"

#include "System/TcpCommandsPermissions/TcpCommandsItem.h"
#include "System/TcpCommandsPermissions/TcpCommandsItemDelegate.h"

#include "QDebug"

TcpCommandsItemModel::TcpCommandsItemModel(QObject *parent) :
    QStandardItemModel(parent)
{

    setHorizontalHeaderLabels( QStringList() << "Befehlsname" << "Berechtigte Gruppe" );
}

void TcpCommandsItemModel::addCommand(const tcpCommand &command) {

    /**
      * Schauen, ob ein Item für diese CommandGroup bereits eingelegt wurde,
      * wenn nicht, legen wir diese an...
      */
    TcpCommandsItem *commandGroupItem;

    // Wurde die Gruppe im Cache bereits angelegt?
    QMap<QString, TcpCommandsItem*>::Iterator itFind = _cacheCommandGroups.find( command.group );
    if (itFind == _cacheCommandGroups.end()) {

        // Gruppe wurde noch nicht angelegt...
        commandGroupItem = new TcpCommandsItem(TcpCommandsItem::COMMANDGROUP, command.group);
        _cacheCommandGroups.insert(command.group, commandGroupItem);

        TcpCommandsItem *emptyItem = new TcpCommandsItem(TcpCommandsItem::EMPTY, "");

        appendRow(QList<QStandardItem*>() << commandGroupItem << emptyItem);
    }
    else {
        // Gruppe wurde im Model bereits angelegt..
        commandGroupItem = (*itFind);
    }


    // Befehlsname:
    TcpCommandsItem *commandItem = new TcpCommandsItem(TcpCommandsItem::COMMAND, command.action);
    commandItem->setId( command.id );


    // Systemgruppe, in welcher sich der Befehl befindet:
    TcpCommandsItem *systemGroupItem = new TcpCommandsItem(TcpCommandsItem::SYSTEMGROUP,
                                                           getSystemGroupNameById(command.systemGroupId));
    systemGroupItem->setId( command.systemGroupId );

    // Tcp- Befehl ins Model einfügen:
    commandGroupItem->appendRow( QList<QStandardItem*>() << commandItem << systemGroupItem );
}

void TcpCommandsItemModel::addSystemGroup(quint32 id, const QString &name) {
    _cacheSystemGroups.insert(id, name);
}

QString TcpCommandsItemModel::getSystemGroupNameById(quint32 id) {

    if (id == 0) {
        return QString("- Keine Gruppe -");
    }

    QMap<quint32, QString>::iterator i = _cacheSystemGroups.find(id);
    if (i == _cacheSystemGroups.end()) {
        return QString("SystemError: group not found!");
    }
    else {
        return i.value();
    }
}

QMap<quint32, QString>& TcpCommandsItemModel::getAllSystemGroups() {
    return _cacheSystemGroups;
}

bool TcpCommandsItemModel::setData(const QModelIndex &index, const QVariant &value, int role) {

    if (role == Qt::UserRole + 1 && index.column() == 1) {

        quint32 commandId = index.sibling(index.row(), 0).data(Qt::UserRole + 1).toInt();
        _changes.insert(commandId, value.toInt());      // value = neue SystemGruppe- ID
    }

    return QStandardItemModel::setData(index, value, role);
}

QHash<quint32, quint32>& TcpCommandsItemModel::getAllChangedSysGroups() {

    return _changes;
}
