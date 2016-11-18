#include "getCustomerInterfaceAddress.h"

bool TcpCommand_backupMonitor_getCustomerInterfaceAddress::_loaded = false;
QString TcpCommand_backupMonitor_getCustomerInterfaceAddress::_address;

TcpCommand_backupMonitor_getCustomerInterfaceAddress::TcpCommand_backupMonitor_getCustomerInterfaceAddress(QObject *parent) :
    TcpAbstractCommand(parent)
{

    if (_loaded) {
        setNoTcpExecution(true);
    }
}

void TcpCommand_backupMonitor_getCustomerInterfaceAddress::execWriteData() {
    
}

void TcpCommand_backupMonitor_getCustomerInterfaceAddress::execReadData() {

    _dataStream >> _address;
    _loaded = true;
}

