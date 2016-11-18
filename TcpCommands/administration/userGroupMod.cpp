#include "TcpCommands/administration/userGroupMod.h"

TcpCommand_administration_userGroupMod::TcpCommand_administration_userGroupMod(QObject *parent)
    : TcpAbstractCommand(parent), _action(UNKNOWN), _id(-1)
{
}

// Daten, welche in den Datenstream geschrieben werden...
// Daten werde in "_dataStream << data" geschrieben...
void TcpCommand_administration_userGroupMod::execWriteData() {

    // Wurde eine Aktion mit ::setAction() ausgewählt??
    //Q_ASSERT(_action == UNKNOWN);

    _dataStream << static_cast<quint32>(_action);
    _dataStream << _id;
    _dataStream << _name;
    _dataStream << _locked;
    _dataStream << _ldapFilter1Enabled;
    _dataStream << _ldapFilter2Enabled;
    _dataStream << _ldapFilter3Enabled;
    _dataStream << _filter1;
    _dataStream << _filter2;
    _dataStream << _filter3;
}

// Daten, welche der Server uns zurÃŒckgesendet hat...
// Daten werden von "_dataStream >> data" gelesen...
void TcpCommand_administration_userGroupMod::execReadData() {

    // Antwort auswerten:
    _dataStream >> _isSuccess;
    _dataStream >> _id;
    _dataStream >> _errorMessage;
}

