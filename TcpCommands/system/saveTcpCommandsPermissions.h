/**
  * Speichert die Systemgruppe zum jeweiligen Tcp- Command.
  */

#ifndef TCPCOMMAND_SYSTEM_SAVETCPCOMMANDSPERMISSIONS_H
#define TCPCOMMAND_SYSTEM_SAVETCPCOMMANDSPERMISSIONS_H

#include <TcpAbstractCommand.h>

#include <QObject>
#include <QString>
#include <QHash>

class TcpCommand_system_saveTcpCommandsPermissions : public TcpAbstractCommand
{
    Q_OBJECT

private:

    // Liste mit allen durchzufüghrenden Änderungen...
    QHash<quint32, quint32> *_changes;

    // Ist ein Fehler beim speichern aufgetreten?
    bool _errorOccurred;
public:

    TcpCommand_system_saveTcpCommandsPermissions(QObject *parent = 0);

    // Setzt die zu speichernden Änderungen:
    // QHash< [commandID], [systemGroup] >
    void setPermissions(QHash<quint32, quint32>* permissions);

    void execWriteData();
    void execReadData();

};

#endif // TCPCOMMAND_SYSTEM_SAVETCPCOMMANDSPERMISSIONS_H


