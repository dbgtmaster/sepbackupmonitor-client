/**
  * Listet eine Liste mit allen Kunden, welche den Backup- Monitor aktiviert haben.
  */

#ifndef TCPCOMMAND_BACKUPMONITOR_GETCUSTOMERS_H
#define TCPCOMMAND_BACKUPMONITOR_GETCUSTOMERS_H

#include <TcpAbstractCommand.h>

#include <QObject>
#include <QString>
#include <QStringList>

class TcpCommand_backupMonitor_getCustomers : public TcpAbstractCommand
{
    Q_OBJECT
    
private:
    QList< QPair<quint32, QString> > _customerList;
public:
    
    TcpCommand_backupMonitor_getCustomers(QObject *parent = 0);
    
    void execWriteData();
    void execReadData();

    const QList< QPair<quint32, QString> >& getCustomerList() const {
        return _customerList;
    }
    
};

#endif // TCPCOMMAND_BACKUPMONITOR_GETCUSTOMERS_H


