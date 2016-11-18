#ifndef TCPCOMMAND_SERVERVERSION_H
#define TCPCOMMAND_SERVERVERSION_H

#include <TcpAbstractCommand.h>

class TcpCommand_main_serverVersion : public TcpAbstractCommand
{
    Q_OBJECT
private:
    QByteArray _serverVersion;
public:
    explicit TcpCommand_main_serverVersion(QObject *parent = 0);

    void execWriteData();
    void execReadData();

    QByteArray &getServerVersion();
public slots:

};

#endif // TCPCOMMAND_SERVERVERSION_H
