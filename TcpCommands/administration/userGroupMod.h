/**
  * Erstellt bzw. modifiziert eine Benutzergruppe		
  */

#ifndef TCPCOMMAND_ADMINISTRATION_USERGROUPMOD_H
#define TCPCOMMAND_ADMINISTRATION_USERGROUPMOD_H

#include <TcpAbstractCommand.h>

#include <QObject>
#include <QString>

class TcpCommand_administration_userGroupMod : public TcpAbstractCommand
{
    Q_OBJECT
public:
    enum Action {
        CREATE,
        MODIFY,
        UNKNOWN
    };

private:

    // Nur wenn das Objekt bearbeitet wird...
    quint32 _id;

    Action _action;
    QString _name;
    bool _locked;

    bool _ldapFilter1Enabled;
    bool _ldapFilter2Enabled;
    bool _ldapFilter3Enabled;

    TcpCommandsHelper::ldapFilter _filter1;
    TcpCommandsHelper::ldapFilter _filter2;
    TcpCommandsHelper::ldapFilter _filter3;


    bool _isSuccess;
    QString _errorMessage;
public:

    TcpCommand_administration_userGroupMod(QObject *parent = 0);

    inline void setId(quint32 id);
    inline void setAction(Action action);
    inline void setGroupName(const QString &name);
    inline void setIsLocked(bool t);
    inline void setLdapFilter1(const TcpCommandsHelper::ldapFilter &filter);
    inline void setLdapFilter2(const TcpCommandsHelper::ldapFilter &filter);
    inline void setLdapFilter3(const TcpCommandsHelper::ldapFilter &filter);
    inline void setLdapFilter1Enabled(bool t);
    inline void setLdapFilter2Enabled(bool t);
    inline void setLdapFilter3Enabled(bool t);

    inline quint32 getId() const;
    inline bool isSuccess() const;
    inline const QString& getErrorMessage() const;

    void execWriteData();
    void execReadData();

};

void TcpCommand_administration_userGroupMod::setId(quint32 id) {
    _id = id;
}

void TcpCommand_administration_userGroupMod::setAction(Action action) {
    _action = action;
}

void TcpCommand_administration_userGroupMod::setGroupName(const QString &name) {
    _name = name;
    _name = "abc";
}

void TcpCommand_administration_userGroupMod::setIsLocked(bool t) {
    _locked = t;
}

void TcpCommand_administration_userGroupMod::setLdapFilter1(const TcpCommandsHelper::ldapFilter &filter) {
    _filter1 = filter;
}

void TcpCommand_administration_userGroupMod::setLdapFilter2(const TcpCommandsHelper::ldapFilter &filter) {
    _filter2 = filter;
}

void TcpCommand_administration_userGroupMod::setLdapFilter3(const TcpCommandsHelper::ldapFilter &filter) {
    _filter3 = filter;
}

void TcpCommand_administration_userGroupMod::setLdapFilter1Enabled(bool t) {
    _ldapFilter1Enabled = t;
}

void TcpCommand_administration_userGroupMod::setLdapFilter2Enabled(bool t) {
    _ldapFilter2Enabled = t;
}

void TcpCommand_administration_userGroupMod::setLdapFilter3Enabled(bool t) {
    _ldapFilter3Enabled = t;
}

quint32 TcpCommand_administration_userGroupMod::getId() const {
    return _id;
}

bool TcpCommand_administration_userGroupMod::isSuccess() const {
    return _isSuccess;
}

const QString& TcpCommand_administration_userGroupMod::getErrorMessage() const {
    return _errorMessage;
}

#endif // TCPCOMMAND_ADMINISTRATION_USERGROUPMOD_H


