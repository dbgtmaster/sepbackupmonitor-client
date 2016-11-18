#include "customerGroupMod.h"

TcpCommand_backupMonitor_customerGroupMod::TcpCommand_backupMonitor_customerGroupMod(QObject *parent) :
    TcpAbstractCommand(parent)
{
}

void TcpCommand_backupMonitor_customerGroupMod::execWriteData() {

    _dataStream << static_cast<quint32>(_action) << _groupId << _cols;
}

void TcpCommand_backupMonitor_customerGroupMod::execReadData() {

    _dataStream >> _isSuccessful >> _errorMessage;
    qDebug() << _errorMessage;
}

