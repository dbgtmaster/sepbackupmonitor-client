#include "getBackupStateOfDay.h"

TcpCommand_backupMonitor_getBackupStateOfDay::TcpCommand_backupMonitor_getBackupStateOfDay(QObject *parent) :
    TcpAbstractCommand(parent)
{
}

void TcpCommand_backupMonitor_getBackupStateOfDay::execWriteData() {

    _dataStream << _dates << _customerId;
}

void TcpCommand_backupMonitor_getBackupStateOfDay::execReadData() {

    _dataStream >> _returnedList;
}

