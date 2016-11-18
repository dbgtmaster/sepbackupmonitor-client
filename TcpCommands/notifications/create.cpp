#include "create.h"

TcpCommand_notifications_create::TcpCommand_notifications_create(QObject *parent) :
    TcpAbstractCommand(parent)
{
}

void TcpCommand_notifications_create::execWriteData() {
    
    _dataStream << _type << _data << _priority;
}

void TcpCommand_notifications_create::execReadData() {

    // Wir erhalten keine Antwort...
}

