#include "TcpCommands/administration/testLDAPGroupMemberships.h"

TcpCommand_administration_testLDAPGroupMemberships::TcpCommand_administration_testLDAPGroupMemberships(QObject *parent)
    : TcpAbstractCommand(parent), _filter1Enabled(false), _filter2Enabled(false), _filter3Enabled(false)
{
}

void TcpCommand_administration_testLDAPGroupMemberships::setUsername(const QString &username) {
    _username = username;
}

void TcpCommand_administration_testLDAPGroupMemberships::setLdapFilter1(const TcpCommandsHelper::ldapFilter &filter) {
    _filter1 = filter;
}

void TcpCommand_administration_testLDAPGroupMemberships::setLdapFilter2(const TcpCommandsHelper::ldapFilter &filter) {
    _filter2 = filter;
}

void TcpCommand_administration_testLDAPGroupMemberships::setLdapFilter3(const TcpCommandsHelper::ldapFilter &filter) {
    _filter3 = filter;
}

// Daten, welche in den Datenstream geschrieben werden...
// Daten werde in "_dataStream << data" geschrieben...
void TcpCommand_administration_testLDAPGroupMemberships::execWriteData() {

    _dataStream << _username << _filter1 << _filter2 << _filter3
                   << _filter1Enabled << _filter2Enabled << _filter3Enabled;
}

// Daten, welche der Server uns zurükgesendet hat...
// Daten werden von "_dataStream >> data" gelesen...
void TcpCommand_administration_testLDAPGroupMemberships::execReadData() {

    _dataStream >> _filter1Success;
    _dataStream >> _filter2Success;
    _dataStream >> _filter3Success;
}

