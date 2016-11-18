#include "BackupMonitorItem.h"

#include <QDebug>

BackupMonitorItem::BackupMonitorItem(const QString &text)
    : QStandardItem(text), _itemType(JOB), _countSuccessful(0), _countWarning(0),
      _countError(0), _countRunning(0)
{

}
