/**
  * Durchsucht die Kunden nach bestimmten Kritierien und liefert eine Liste mit den Suchergebnissen zurÃŒck.
  */

#ifndef TCPCOMMAND_CUSTOMERS_SEARCH_H
#define TCPCOMMAND_CUSTOMERS_SEARCH_H

#include <TcpAbstractCommand.h>

#include <QObject>
#include <QString>
#include <QStringList>

class TcpCommand_customers_search : public TcpAbstractCommand
{
    Q_OBJECT
    
private:
    QStringList _searchFilter;
    QStringList _filterColumns;
    QStringList _returnColumns;

    QList< QHash<QString, QVariant> > _returnedCustomers;
    int _hasNextCounter;
public:
    
    TcpCommand_customers_search(QObject *parent = 0);
    
    void execWriteData();
    void execReadData();
    
    // Setzt den Suchfilter:
    inline void setSearchFilter(const QString &s);
    inline void setSearchFilter(const QStringList &s);

    // In welchen Spalten soll gefiltert werden (customer_name, address,...)
    inline void setFilterColumns(const QStringList &s);

    // Welche Spalten sollen zurückgegeben werden...
    inline void setReturnColumns(const QStringList &s);

    // Gibt den nächst gefundenen Kunden zurückn...
    bool hasNextCustomer();
    QHash<QString, QVariant> fetchNextCustomer();
    inline int getCountEntries();
};

void TcpCommand_customers_search::setSearchFilter(const QString &s) {
    _searchFilter << s;
}

void TcpCommand_customers_search::setSearchFilter(const QStringList &s) {
    _searchFilter = s;
}

void TcpCommand_customers_search::setFilterColumns(const QStringList &s) {
    _filterColumns = s;
}

void TcpCommand_customers_search::setReturnColumns(const QStringList &s) {
    _returnColumns = s;
}

int TcpCommand_customers_search::getCountEntries() {
    return _returnedCustomers.count();
}

#endif // TCPCOMMAND_CUSTOMERS_SEARCH_H


