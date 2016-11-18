/**
  * ÃberprÃŒft die LDAP Einstellungen einer Gruppe anhand eines Benutzers...
  */

#ifndef TCPCOMMAND_ADMINISTRATION_TESTLDAPGROUPMEMBERSHIPS_H
#define TCPCOMMAND_ADMINISTRATION_TESTLDAPGROUPMEMBERSHIPS_H

#include <TcpAbstractCommand.h>

#include <QObject>
#include <QString>

class TcpCommand_administration_testLDAPGroupMemberships : public TcpAbstractCommand
{
    Q_OBJECT

private:
    QString _username;
    TcpCommandsHelper::ldapFilter _filter1;
    TcpCommandsHelper::ldapFilter _filter2;
    TcpCommandsHelper::ldapFilter _filter3;
    bool _filter1Enabled;
    bool _filter2Enabled;
    bool _filter3Enabled;

    // Und ob Filter erfolgreich war oder auch nicht...
    bool _filter1Success;
    bool _filter2Success;
    bool _filter3Success;
public:

    // Einstellungen, mit welchen geprüft werden soll...
    void setUsername(const QString &username);
    void setLdapFilter1(const TcpCommandsHelper::ldapFilter &filter);
    void setLdapFilter2(const TcpCommandsHelper::ldapFilter &filter);
    void setLdapFilter3(const TcpCommandsHelper::ldapFilter &filter);
    inline void setLdapFilter1Enabled(bool t) { _filter1Enabled = t; }
    inline void setLdapFilter2Enabled(bool t) { _filter2Enabled = t; }
    inline void setLdapFilter3Enabled(bool t) { _filter3Enabled = t; }

    TcpCommand_administration_testLDAPGroupMemberships(QObject *parent = 0);

    void execWriteData();
    void execReadData();

    inline bool filter1Success() { return _filter1Success; }
    inline bool filter2Success() { return _filter2Success; }
    inline bool filter3Success() { return _filter3Success; }
};

#endif // TCPCOMMAND_ADMINISTRATION_TESTLDAPGROUPMEMBERSHIPS_H


