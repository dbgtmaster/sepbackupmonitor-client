/**
  * Ermittelt Daten zu einer berstimmten Benutzer- Gruppen
  */

#ifndef TCPCOMMAND_ADMINISTRATION_GETUSERGROUP_H
#define TCPCOMMAND_ADMINISTRATION_GETUSERGROUP_H

#include <TcpAbstractCommand.h>

#include <QObject>
#include <QString>
#include <QStringList>

class TcpCommand_administration_getUserGroup : public TcpAbstractCommand
{
    Q_OBJECT

    // Request:
    quint32 _id;
    QStringList _columns;

    // Ob auch die Zuordnung zu den Systemgruppen ermittelt werden soll:
    bool _getSystemGroupsMembership;

    // Response:
    mapField _returnedColumns;
    QList<quint32> _returnedSystemGroupsMembership;

public:

    TcpCommand_administration_getUserGroup(QObject *parent = 0);

    inline void setId(quint32 id);
    inline void setColumns(const QStringList &columns);

    // Wenn true, wenn wird bei der Antwort des Servers auch die Mitgliedschaft
    // der Gruppe zu den Systemgruppen zurückgegeben (-> getSystemGroupsMembership() ).
    inline void setGetSystemMemberships(bool t);

    void execWriteData();
    void execReadData();

    inline bool exists();
    inline QVariant getColumn(const QString &name);

    // Um die LDAP- Filter lesen zu können, müssen folgende Datenbank- Spalten gelesen worden sein:
    // ldapfilter1_scope, ldapfilter1_dn, ldapfilter1_filter -> Für LDAPFilter 1
    TcpCommandsHelper::ldapFilter getLdapFilter1();
    TcpCommandsHelper::ldapFilter getLdapFilter2();
    TcpCommandsHelper::ldapFilter getLdapFilter3();

    inline QList<quint32> getSystemGroupsMembership();
};

void TcpCommand_administration_getUserGroup::setId(quint32 id) {
    _id = id;
}

void TcpCommand_administration_getUserGroup::setColumns(const QStringList &columns) {
    _columns = columns;
}

void TcpCommand_administration_getUserGroup::setGetSystemMemberships(bool t) {
    _getSystemGroupsMembership = t;
}

bool TcpCommand_administration_getUserGroup::exists() {
    return !_returnedColumns.isEmpty();
}

QVariant TcpCommand_administration_getUserGroup::getColumn(const QString &name) {
    return _returnedColumns[name];
}

QList<quint32> TcpCommand_administration_getUserGroup::getSystemGroupsMembership() {

    return _returnedSystemGroupsMembership;
}

#endif // TCPCOMMAND_ADMINISTRATION_GETUSERGROUP_H


