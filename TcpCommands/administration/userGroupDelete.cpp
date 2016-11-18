#include "TcpCommands/administration/userGroupDelete.h"

TcpCommand_administration_userGroupDelete::TcpCommand_administration_userGroupDelete(QObject *parent) : TcpAbstractCommand(parent)
{
}

// Daten, welche in den Datenstream geschrieben werden...
// Daten werde in "_dataStream << data" geschrieben...
void TcpCommand_administration_userGroupDelete::execWriteData() {

    _dataStream << _id;
}

// Daten, welche der Server uns zurÃŒckgesendet hat...
// Daten werden von "_dataStream >> data" gelesen...
void TcpCommand_administration_userGroupDelete::execReadData() {

    _dataStream >> _successful;
}

