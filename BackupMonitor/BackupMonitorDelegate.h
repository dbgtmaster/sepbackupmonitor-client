#ifndef BACKUPMONITORDELEGATE_H
#define BACKUPMONITORDELEGATE_H

#include <QStyledItemDelegate>

#include <QPixmap>

class BackupMonitorDelegate : public QStyledItemDelegate
{
    Q_OBJECT

private:
    QPixmap _pixStateRed;
    QPixmap _pixStateGreen;
    QPixmap _pixStateYellow;
    QPixmap _pixStateBlue;

public:
    explicit BackupMonitorDelegate(QObject *parent = 0);

    void paint(QPainter *painter,
          const QStyleOptionViewItem &option,
          const QModelIndex &index) const;

signals:
    
public slots:
    
};

#endif // BACKUPMONITORDELEGATE_H
