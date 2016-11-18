#ifndef TCPCOMMAND__BACKUPMONITOR_GETCUSTOMERGROUPS_H
#define TCPCOMMAND__BACKUPMONITOR_GETCUSTOMERGROUPS_H

#include <TcpAbstractCommand.h>

#include <QStringList>

class TcpCommand_backupMonitor_getCustomerGroups : public TcpAbstractCommand
{
    Q_OBJECT
private:
    QStringList _parameters;
    quint32 _filterId;
    
    QList< QHash<QString, QVariant > > _returnedGroups;

public:
    explicit TcpCommand_backupMonitor_getCustomerGroups(QObject *parent = 0);
    
    void execWriteData();
    void execReadData();
    
    void setParameters(const QStringList &list) { _parameters = list; }
    void setFilterId(const quint32 id) { _filterId = id; }

    int total() { return _returnedGroups.count(); }
    bool hasNextCustomerGroup();
    QHash<QString, QVariant> fetchNextCustomerGroup();

    QList< QHash<QString, QVariant > > getAllCustomerGroups() {
        return _returnedGroups;
    }

public slots:
    
};

#endif // TCPCOMMAND__BACKUPMONITOR_GETCUSTOMERGROUPS_H

