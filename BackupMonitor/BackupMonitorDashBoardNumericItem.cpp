#include "BackupMonitorDashBoardNumericItem.h"

BackupMonitorDashBoardNumericItem::BackupMonitorDashBoardNumericItem(int type) : QTableWidgetItem(type)
{
}

BackupMonitorDashBoardNumericItem::BackupMonitorDashBoardNumericItem(const QString &text, int type) : QTableWidgetItem(text, type) {
}

BackupMonitorDashBoardNumericItem::BackupMonitorDashBoardNumericItem(const QIcon &icon, const QString &text, int type) : QTableWidgetItem(icon, text, type) {
}

bool BackupMonitorDashBoardNumericItem::operator <(const QTableWidgetItem &other) const
{
    QString str1 = text();
    QString str2 = other.text();

    return str1.split(" ")[0].toDouble() < str2.split(" ")[0].toDouble();
}
