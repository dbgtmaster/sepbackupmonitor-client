#include "TcpCommands/administration/getAllUserGroups.h"

#include <QStringList>

TcpCommand_administration_getAllUserGroups::TcpCommand_administration_getAllUserGroups(QObject *parent) : TcpAbstractCommand(parent)
{
}

void TcpCommand_administration_getAllUserGroups::setColumns(const QStringList &columns) {
    _columns = columns;
}

// Daten, welche in den Datenstream geschrieben werden...
// Daten werde in "_dataStream << data" geschrieben...
void TcpCommand_administration_getAllUserGroups::execWriteData() {

    _dataStream << _columns;
}

// Daten, welche der Server uns zurÃŒckgesendet hat...
// Daten werden von "_dataStream >> data" gelesen...
void TcpCommand_administration_getAllUserGroups::execReadData() {

}

bool TcpCommand_administration_getAllUserGroups::hasNext() {

    if (_dataStream.atEnd()) return false;

    // Neuen Daten auslesen:
    _dataStream >> _actRow;

    return true;
}

QVariant TcpCommand_administration_getAllUserGroups::getColumn(const QString &name) {
    return _actRow[name];
}

