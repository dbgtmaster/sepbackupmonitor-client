/**
  * Gibt eine Liste mit allen Benutzergruppen zurück.
  */

#ifndef TCPCOMMAND_ADMINISTRATION_GETALLUSERGROUPS_H
#define TCPCOMMAND_ADMINISTRATION_GETALLUSERGROUPS_H

#include <TcpAbstractCommand.h>

#include <QObject>
#include <QString>
#include <QStringList>

class TcpCommand_administration_getAllUserGroups : public TcpAbstractCommand
{
    Q_OBJECT

private:
    // Auszulesende Spalten:
    QStringList _columns;

    // Aktuell auszulesende Gruppe:
    mapField _actRow;

public:

    TcpCommand_administration_getAllUserGroups(QObject *parent = 0);

    void setColumns(const QStringList &columns);

    void execWriteData();
    void execReadData();

    bool hasNext();

    QVariant getColumn(const QString &name);
};

#endif // TCPCOMMAND_ADMINISTRATION_GETALLUSERGROUPS_H


