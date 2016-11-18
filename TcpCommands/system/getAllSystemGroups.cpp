#include "TcpCommands/system/getAllSystemGroups.h"

TcpCommand_system_getAllSystemGroups::TcpCommand_system_getAllSystemGroups(QObject *parent)
    : TcpAbstractCommand(parent), _getDescription(false)
{
}

// Daten, welche in den Datenstream geschrieben werden...
// Daten werde in "_dataStream << data" geschrieben...
void TcpCommand_system_getAllSystemGroups::execWriteData() {

    // Soll Server auch Beschreibung der Systemgruppe liefern?
    _dataStream << _getDescription;
}

// Daten, welche der Server uns zurÃŒckgesendet hat...
// Daten werden von "_dataStream >> data" gelesen...
void TcpCommand_system_getAllSystemGroups::execReadData() {

}

bool TcpCommand_system_getAllSystemGroups::hasNext() {

    if (_dataStream.atEnd()) return false;

    // Neuen Daten auslesen:
    _dataStream >> _groupId;
    _dataStream >> _groupName;

    if (_getDescription)
        _dataStream >> _description;

    return true;
}
