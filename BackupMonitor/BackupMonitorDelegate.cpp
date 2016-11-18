#include "BackupMonitorDelegate.h"

#include "BackupMonitor/BackupMonitorItemModel.h"
#include "BackupMonitor/BackupMonitorItem.h"

#include <QPainter>

#include <QDebug>

BackupMonitorDelegate::BackupMonitorDelegate(QObject *parent) :
    QStyledItemDelegate(parent), _pixStateGreen(":/green.png"),
    _pixStateYellow(":/yellow.png"), _pixStateRed(":/red.png"),
    _pixStateBlue(":/blue.png")
{
}

void BackupMonitorDelegate::paint(QPainter *painter,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const {

    BackupMonitorItem* item =
            static_cast<BackupMonitorItem*>(
                    static_cast<const BackupMonitorItemModel*>(index.model())
                        ->itemFromIndex(index)
                );

    if (item == 0) {
        return QStyledItemDelegate::paint(painter, option, index);
    }

    if (item->_itemType == BackupMonitorItem::MAIN_GROUP && index.column() == 1) {

        QStyledItemDelegate::paint(painter, option, index);

        int width = _pixStateGreen.width();
        int height = _pixStateGreen.height();
        int x = option.rect.x();
        int y = option.rect.y() + 2;

        painter->setPen(Qt::black);
        painter->setFont(QFont("Arial", 8));

        painter->drawPixmap(x, y, _pixStateGreen);
        x = x + width + 1;
        painter->drawText(x, y + (option.rect.height() / 2) + 2, QString::number(item->_countSuccessful));
        x = x + 21;

        painter->drawPixmap(x, y, _pixStateYellow);
        x = x + width + 1;
        painter->drawText(x, y + (option.rect.height() / 2 + 2), QString::number(item->_countWarning));
        x = x + 21;

        painter->drawPixmap(x, y, _pixStateRed);
        x = x + width + 1;
        painter->drawText(x, y + (option.rect.height() / 2) + 2, QString::number(item->_countError));
        x = x + 21;

        painter->drawPixmap(x, y, _pixStateBlue);
        x = x + width + 1;
        painter->drawText(x, y + (option.rect.height() / 2) + 2, QString::number(item->_countRunning));

    }
    else {
        return QStyledItemDelegate::paint(painter, option, index);
    }
}
