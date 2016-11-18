#ifndef BACKUPMONITORCLIENTSTATUSSORTFILTERPROXY_H
#define BACKUPMONITORCLIENTSTATUSSORTFILTERPROXY_H

#include <QSortFilterProxyModel>

class BackupMonitorClientStatusSortFilterProxy : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    BackupMonitorClientStatusSortFilterProxy(QObject *parent = 0);

protected:
    //bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};

#endif // BACKUPMONITORCLIENTSTATUSSORTFILTERPROXY_H
