#include "TcpCommands/system/getAllTcpCommands.h"

TcpCommand_system_getAllTcpCommands::TcpCommand_system_getAllTcpCommands(QObject *parent) : TcpAbstractCommand(parent)
{
}

// Daten, welche in den Datenstream geschrieben werden...
// Daten werde in "_dataStream << data" geschrieben...
void TcpCommand_system_getAllTcpCommands::execWriteData() {

    // Wir brauchen nichts senden...
}

// Daten, welche der Server uns zurückgesendet hat...
// Daten werden von "_dataStream >> data" gelesen...
void TcpCommand_system_getAllTcpCommands::execReadData() {

}

bool TcpCommand_system_getAllTcpCommands::hasNext() {

    if (_dataStream.atEnd()) return false;

    // Neuen Daten auslesen:
    _dataStream >> _commandId;
    _dataStream >> _commandGroup;
    _dataStream >> _commandName;
    _dataStream >> _systemGroupId;

    return true;
}
