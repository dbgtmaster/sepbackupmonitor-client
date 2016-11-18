#include "customerGroupDelete.h"

TcpCommand_backupMonitor_customerGroupDelete::TcpCommand_backupMonitor_customerGroupDelete(QObject *parent) :
    TcpAbstractCommand(parent)
{
}

void TcpCommand_backupMonitor_customerGroupDelete::execWriteData() {
    
    _dataStream << _id;
}

void TcpCommand_backupMonitor_customerGroupDelete::execReadData() {
}

