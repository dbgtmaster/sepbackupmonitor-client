#include "getCustomerGroups.h"

TcpCommand_backupMonitor_getCustomerGroups::TcpCommand_backupMonitor_getCustomerGroups(QObject *parent) :
    TcpAbstractCommand(parent)
{
    // Standardmäßig filtern wir nicht..
    _filterId = 0;
}

void TcpCommand_backupMonitor_getCustomerGroups::execWriteData() {
    
    _dataStream << _parameters << _filterId;

}

void TcpCommand_backupMonitor_getCustomerGroups::execReadData() {

    _dataStream >> _returnedGroups;
}


QHash<QString, QVariant> TcpCommand_backupMonitor_getCustomerGroups::fetchNextCustomerGroup() {

    return _returnedGroups.takeFirst();
}


bool TcpCommand_backupMonitor_getCustomerGroups::hasNextCustomerGroup() {

    return (_returnedGroups.count() != 0);
}
