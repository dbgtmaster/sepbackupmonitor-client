#include "TcpCommands/administration/getUserGroup.h"

TcpCommand_administration_getUserGroup::TcpCommand_administration_getUserGroup(QObject *parent)
    : TcpAbstractCommand(parent), _getSystemGroupsMembership(false)
{
}

// Daten, welche in den Datenstream geschrieben werden...
// Daten werde in "_dataStream << data" geschrieben...
void TcpCommand_administration_getUserGroup::execWriteData() {

    _dataStream << _id;
    _dataStream << _columns;
    _dataStream << _getSystemGroupsMembership;
}

// Daten, welche der Server uns zurückgesendet hat...
// Daten werden von "_dataStream >> data" gelesen...
void TcpCommand_administration_getUserGroup::execReadData() {

    _dataStream >> _returnedColumns;
    _dataStream >> _returnedSystemGroupsMembership;
}

TcpCommandsHelper::ldapFilter TcpCommand_administration_getUserGroup::getLdapFilter1() {

    TcpCommandsHelper::ldapFilter f;
    f.baseDn = getColumn("ldapfilter1_dn").toString();
    f.scope = static_cast<TcpCommandsHelper::ldapScope>( getColumn("ldapfilter1_scope").toInt() );
    f.filter = getColumn("ldapfilter1_filter").toString();

    return f;
}

TcpCommandsHelper::ldapFilter TcpCommand_administration_getUserGroup::getLdapFilter2() {

    TcpCommandsHelper::ldapFilter f;
    f.baseDn = getColumn("ldapfilter2_dn").toString();
    f.scope = static_cast<TcpCommandsHelper::ldapScope>( getColumn("ldapfilter2_scope").toInt() );
    f.filter = getColumn("ldapfilter2_filter").toString();

    return f;
}

TcpCommandsHelper::ldapFilter TcpCommand_administration_getUserGroup::getLdapFilter3() {

    TcpCommandsHelper::ldapFilter f;
    f.baseDn = getColumn("ldapfilter3_dn").toString();
    f.scope = static_cast<TcpCommandsHelper::ldapScope>( getColumn("ldapfilter3_scope").toInt() );
    f.filter = getColumn("ldapfilter3_filter").toString();

    return f;
}
