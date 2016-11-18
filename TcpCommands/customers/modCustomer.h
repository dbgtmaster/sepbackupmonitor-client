/**
  * Bearbeitet einen Kunden
  */

#ifndef TCPCOMMAND_CUSTOMERS_MODCUSTOMER_H
#define TCPCOMMAND_CUSTOMERS_MODCUSTOMER_H

#include <TcpAbstractCommand.h>

#include <QObject>
#include <QString>

class TcpCommand_customers_modCustomer : public TcpAbstractCommand
{
    Q_OBJECT
    
    quint32 _customerId;
    QHash<QString, QVariant> _modColumns;

    QVariant _modStatus;

public:
    
    TcpCommand_customers_modCustomer(QObject *parent = 0);
    
    inline void setCustomerId(quint32 id) {
        _customerId = id;
    }

    inline quint32 getCustomerId() {
        return _customerId;
    }

    inline void setModColumns(const QHash<QString, QVariant> &mod) {
        _modColumns = mod;
    }

    inline bool successfulModified() {
        return ( _modStatus.type() == QVariant::Bool && _modStatus.toBool() );
    }

    inline QString getErrorMessage() {
        return _modStatus.toString();
    }

    void execWriteData();
    void execReadData();
    
};

#endif // TCPCOMMAND_CUSTOMERS_MODCUSTOMER_H


