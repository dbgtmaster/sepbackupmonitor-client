#ifndef TCPCOMMAND__SYSTEM_SERVERSTATISTICS_H
#define TCPCOMMAND__SYSTEM_SERVERSTATISTICS_H

#include <TcpAbstractCommand.h>

class TcpCommand_system_serverStatistics : public TcpAbstractCommand
{
    Q_OBJECT
private:
    
    QHash<QString, QVariant> _serverData;

public:
    explicit TcpCommand_system_serverStatistics(QObject *parent = 0);
    
    void execWriteData();
    void execReadData();

    inline QHash<QString, QVariant> getServerData();
    
public slots:
    
};

QHash<QString, QVariant> TcpCommand_system_serverStatistics::getServerData() {
    return _serverData;
}

#endif // TCPCOMMAND__SYSTEM_SERVERSTATISTICS_H

