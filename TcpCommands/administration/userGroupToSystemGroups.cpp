#include "TcpCommands/administration/userGroupToSystemGroups.h"

TcpCommand_administration_userGroupToSystemGroups::TcpCommand_administration_userGroupToSystemGroups(QObject *parent) : TcpAbstractCommand(parent)
{
}

// Daten, welche in den Datenstream geschrieben werden...
// Daten werde in "_dataStream << data" geschrieben...
void TcpCommand_administration_userGroupToSystemGroups::execWriteData() {

    _dataStream << _groupId << _memberships;
}

// Daten, welche der Server uns zurÃŒckgesendet hat...
// Daten werden von "_dataStream >> data" gelesen...
void TcpCommand_administration_userGroupToSystemGroups::execReadData() {

}

