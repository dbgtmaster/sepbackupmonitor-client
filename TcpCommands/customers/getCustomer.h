/**
  * Liest Informationen ÃŒber einen Kunden aus und gibt diese zurÃŒck...
  */

#ifndef TCPCOMMAND_CUSTOMERS_GETCUSTOMER_H
#define TCPCOMMAND_CUSTOMERS_GETCUSTOMER_H

#include <TcpAbstractCommand.h>

#include <QObject>
#include <QString>
#include <QStringList>
#include <QHash>

class TcpCommand_customers_getCustomer : public TcpAbstractCommand
{
    Q_OBJECT
    
private:

    int _customerId;
    QStringList _columns;

    QHash<QString, QVariant> _row;

public:
    
    TcpCommand_customers_getCustomer(QObject *parent = 0);
    
    void execWriteData();
    void execReadData();

    inline void setCustomerId(int id);
    inline void setColumns(const QStringList &cols);

    inline QHash<QString, QVariant> fetchRow();
};

void TcpCommand_customers_getCustomer::setCustomerId(int id) {
    _customerId = id;
}

void TcpCommand_customers_getCustomer::setColumns(const QStringList &cols) {
    _columns = cols;
}

QHash<QString, QVariant> TcpCommand_customers_getCustomer::fetchRow() {
    return _row;
}

#endif // TCPCOMMAND_CUSTOMERS_GETCUSTOMER_H
