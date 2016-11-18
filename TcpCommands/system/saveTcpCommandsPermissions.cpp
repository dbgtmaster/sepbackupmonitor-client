#include "TcpCommands/system/saveTcpCommandsPermissions.h"

TcpCommand_system_saveTcpCommandsPermissions::TcpCommand_system_saveTcpCommandsPermissions(QObject *parent)
    : TcpAbstractCommand(parent), _errorOccurred(false)
{
}

void TcpCommand_system_saveTcpCommandsPermissions::setPermissions(QHash<quint32, quint32>* permissions) {

    _changes = permissions;
}

// Daten, welche in den Datenstream geschrieben werden...
// Daten werde in "_dataStream << data" geschrieben...
void TcpCommand_system_saveTcpCommandsPermissions::execWriteData() {

    QHash<quint32, quint32>::iterator it;
    for (it = _changes->begin(); it != _changes->end(); it++) {

        _dataStream << it.key() << it.value();
    }
}

// Daten, welche der Server uns zurÃŒckgesendet hat...
// Daten werden von "_dataStream >> data" gelesen...
void TcpCommand_system_saveTcpCommandsPermissions::execReadData() {

    _dataStream >> _errorOccurred;
}

