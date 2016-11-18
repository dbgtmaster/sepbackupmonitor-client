/**
  * Generiert einen neuen Key fÃŒr das Backup- Monitor Interface.
  */

#ifndef TCPCOMMAND_CUSTOMERS_GENERATENEWBACKUPMONITORKEY_H
#define TCPCOMMAND_CUSTOMERS_GENERATENEWBACKUPMONITORKEY_H

#include <TcpAbstractCommand.h>

#include <QObject>
#include <QString>

class TcpCommand_customers_generateNewBackupMonitorKey : public TcpAbstractCommand
{
    Q_OBJECT

    quint32 _customerId;

    QString _newInterfaceKey;
    
public:
    
    TcpCommand_customers_generateNewBackupMonitorKey(QObject *parent = 0);
    
    void execWriteData();
    void execReadData();
    
    inline void setCustomerId(quint32 id);

    inline QString getNewInterfaceKey();

};

void TcpCommand_customers_generateNewBackupMonitorKey::setCustomerId(quint32 id) {
    _customerId = id;
}

QString TcpCommand_customers_generateNewBackupMonitorKey::getNewInterfaceKey() {
    return _newInterfaceKey;
}

#endif // TCPCOMMAND_CUSTOMERS_GENERATENEWBACKUPMONITORKEY_H


