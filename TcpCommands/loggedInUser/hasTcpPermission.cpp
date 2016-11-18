#include "hasTcpPermission.h"

TcpCommand_loggedInUser_hasTcpPermission::TcpCommand_loggedInUser_hasTcpPermission(QObject *parent) :
    TcpAbstractCommand(parent)
{

}

void TcpCommand_loggedInUser_hasTcpPermission::execWriteData() {
    
    _dataStream << _group << _action;
}

void TcpCommand_loggedInUser_hasTcpPermission::execReadData() {

    _dataStream >> _hasPermission;
    qDebug() << _hasPermission;
}


bool TcpCommands::hasTcpPermission(const QString &group, const QString &action) {

    CREATE_TCP_COMMAND(permissions, loggedInUser, hasTcpPermission);
    permissions->setData(group, action);
    permissions->startWait();
    return permissions->hasPermission();
}
