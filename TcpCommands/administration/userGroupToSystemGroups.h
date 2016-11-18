/**
  * Verwaltet die Mitgliedschaft von einer Gruppe zu Systemgruppen
  */

#ifndef TCPCOMMAND_ADMINISTRATION_USERGROUPTOSYSTEMGROUPS_H
#define TCPCOMMAND_ADMINISTRATION_USERGROUPTOSYSTEMGROUPS_H

#include <TcpAbstractCommand.h>

#include <QObject>
#include <QString>

class TcpCommand_administration_userGroupToSystemGroups : public TcpAbstractCommand
{
    Q_OBJECT

private:
    quint32 _groupId;
    QList<quint32> _memberships;

public:

    TcpCommand_administration_userGroupToSystemGroups(QObject *parent = 0);

    void execWriteData();
    void execReadData();

    inline void setGroupId(quint32 id);
    inline void setMemberships(const QList<quint32> &list);

};

void TcpCommand_administration_userGroupToSystemGroups::setGroupId(quint32 id) {
    _groupId = id;
}

void TcpCommand_administration_userGroupToSystemGroups::setMemberships(const QList<quint32> &list) {
    _memberships = list;
}

#endif // TCPCOMMAND_ADMINISTRATION_USERGROUPTOSYSTEMGROUPS_H


