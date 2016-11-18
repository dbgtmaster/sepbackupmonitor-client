/**
  * Gibt eine QList mit allen SystemgruppenIds + deren Namen zurÃŒck.
  */

#ifndef TCPCOMMAND_SYSTEM_GETALLSYSTEMGROUPS_H
#define TCPCOMMAND_SYSTEM_GETALLSYSTEMGROUPS_H

#include <TcpAbstractCommand.h>

#include <QObject>

#include <QList>
#include <QPair>
#include <QString>

class TcpCommand_system_getAllSystemGroups : public TcpAbstractCommand
{
    Q_OBJECT
private:

    // Soll auch die Beschreibung der Systemgruppe vom Server geholt werden?
    bool _getDescription;

    // Die zuletzt ausgelesenen Daten:
    QString _groupName;
    quint32 _groupId;
    QString _description;
public:

    TcpCommand_system_getAllSystemGroups(QObject *parent = 0);

    // Soll auch die Beschreibung der Systemgruppe vom Server geholt werden?
    inline void setGetDescription(bool t);

    void execWriteData();
    void execReadData();

    inline QString& groupName();
    inline quint32 groupId();

    // Nur wenn Beschreibung aktiviert wurde...
    inline QString &description();

    bool hasNext();
};

void TcpCommand_system_getAllSystemGroups::setGetDescription(bool t) {
    _getDescription = t;
}

QString& TcpCommand_system_getAllSystemGroups::groupName() {
    return _groupName;
}

quint32 TcpCommand_system_getAllSystemGroups::groupId() {
    return _groupId;
}

QString& TcpCommand_system_getAllSystemGroups::description() {
    return _description;
}

#endif // TCPCOMMAND_SYSTEM_GETALLSYSTEMGROUPS_H


