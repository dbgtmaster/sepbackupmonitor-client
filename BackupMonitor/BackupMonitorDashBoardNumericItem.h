#ifndef BACKUPMONITORDASHBOARDNUMERICITEM_H
#define BACKUPMONITORDASHBOARDNUMERICITEM_H

#include <QTableWidgetItem>

class BackupMonitorDashBoardNumericItem : public QTableWidgetItem
{
public:
    explicit BackupMonitorDashBoardNumericItem(int type = Type);
    explicit BackupMonitorDashBoardNumericItem(const QString &text, int type = Type);
    explicit BackupMonitorDashBoardNumericItem(const QIcon &icon, const QString &text, int type = Type);

    bool operator <(const QTableWidgetItem &other) const;
};

#endif // BACKUPMONITORDASHBOARDNUMERICITEM_H
