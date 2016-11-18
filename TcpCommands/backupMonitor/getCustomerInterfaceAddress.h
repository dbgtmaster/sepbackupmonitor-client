#ifndef TCPCOMMAND__BACKUPMONITOR_GETCUSTOMERINTERFACEADDRESS_H
#define TCPCOMMAND__BACKUPMONITOR_GETCUSTOMERINTERFACEADDRESS_H

#include <TcpAbstractCommand.h>

class TcpCommand_backupMonitor_getCustomerInterfaceAddress : public TcpAbstractCommand
{
    Q_OBJECT
private:
    
    static QString _address;
    static bool _loaded;        // Ob der Wert bereits geladen wurde?
public:
    explicit TcpCommand_backupMonitor_getCustomerInterfaceAddress(QObject *parent = 0);
    
    void execWriteData();
    void execReadData();
    
    inline QString getAddress() {
        return _address;
    }

public slots:
    
};

#endif // TCPCOMMAND__BACKUPMONITOR_GETCUSTOMERINTERFACEADDRESS_H

