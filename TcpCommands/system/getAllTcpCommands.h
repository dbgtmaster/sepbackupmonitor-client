/**
  * Gibt eine Liste mit allen verfÃŒgbaren Tcp- Commands zurÃŒck...
  */

#ifndef TCPCOMMAND_SYSTEM_GETALLTCPCOMMANDS_H
#define TCPCOMMAND_SYSTEM_GETALLTCPCOMMANDS_H

#include <TcpAbstractCommand.h>

#include <QObject>
#include <QByteArray>
#include <QPair>

class TcpCommand_system_getAllTcpCommands : public TcpAbstractCommand
{
    Q_OBJECT

private:

    // Die Daten des zuletzt gelesenen Tcp- Befehls:
    int _commandId;
    QByteArray _commandGroup;
    QByteArray _commandName;
    int _systemGroupId;
public:

    TcpCommand_system_getAllTcpCommands(QObject *parent = 0);

    void execWriteData();
    void execReadData();

    // Überprüft, ob weitere Daten vorhandne sind...
    // Falls ja, werden diese geladen...
    bool hasNext();


    // Die Daten des zuletzt gelesenen Tcp- Befehls:
    inline int systemGroupId() const;
    inline int commandId() const;
    inline QByteArray& commandGroup();
    inline QByteArray& commandName();
};

int TcpCommand_system_getAllTcpCommands::systemGroupId() const {
    return _systemGroupId;
}

int TcpCommand_system_getAllTcpCommands::commandId() const {
    return _commandId;
}

QByteArray& TcpCommand_system_getAllTcpCommands::commandGroup() {
    return _commandGroup;
}

QByteArray& TcpCommand_system_getAllTcpCommands::commandName() {
    return _commandName;
}

#endif // TCPCOMMAND_SYSTEM_GETALLTCPCOMMANDS_H


