#include "getLastMessages.h"

TcpCommand_notifications_getLastMessages::TcpCommand_notifications_getLastMessages(QObject *parent) :
    TcpAbstractCommand(parent)
{
}

void TcpCommand_notifications_getLastMessages::execWriteData() {
    
}

void TcpCommand_notifications_getLastMessages::execReadData() {

    _dataStream >> _rows;
}

