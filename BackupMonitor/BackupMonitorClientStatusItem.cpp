#include "BackupMonitorClientStatusItem.h"

#include <QDebug>

BackupMonitorClientStatusItem::BackupMonitorClientStatusItem(const QString &text, TYPE type) :
    QStandardItem(text), _item(ITEM_UNKNOWN)
{
    setEditable(false);
    setItemType(type);
}

void BackupMonitorClientStatusItem::setItem(ITEM item) {
    _item = item;
}

BackupMonitorClientStatusItem::ITEM BackupMonitorClientStatusItem::getItem() {
    return _item;
}

void BackupMonitorClientStatusItem::setItemData(const QVariant &data) {
    _itemData = data;
}

QVariant BackupMonitorClientStatusItem::getItemData() {
    return _itemData;
}
