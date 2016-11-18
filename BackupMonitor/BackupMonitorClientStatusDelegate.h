#ifndef BACKUPMONITORCLIENTSTATUSDELEGATE_H
#define BACKUPMONITORCLIENTSTATUSDELEGATE_H

#include <QStyledItemDelegate>
#include <QTreeView>
#include <QGridLayout>
#include <QLabel>

#include <BackupMonitor/BackupMonitorClientStatus.h>
#include <BackupMonitor/BackupMonitorClientStatusMousePopup.h>

class BackupMonitorClientStatusDelegate : public QStyledItemDelegate
{
        Q_OBJECT

private:
    QPixmap _pixStateRed;
    QPixmap _pixStateRedInactive;
    QPixmap _pixStateGreen;
    QPixmap _pixStateGreenInactive;
    QPixmap _pixStateYellow;
    QPixmap _pixStateYellowInactive;
    QPixmap _pixStateBlue;
    QPixmap _pixStateBlueInactive;
    QPixmap _pixStatePurple;
    QPixmap _pixStatePurpleInactive;

    QTreeView* _view;
    BackupMonitorClientStatus* _clientStatusWindow;

    BackupMonitorClientStatusMousePopup* _mousePopupClientStatus;
    bool _mousePopupClientStatusClicked;        // Ob das Popup dauerhaft auf einer Position angezeigt werden soll?
                                                // Tritt ein, wenn auf einen Tag geklickt wird.

protected:

    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
    bool eventFilter(QObject *object, QEvent *event);

public:
    BackupMonitorClientStatusDelegate(QTreeView *view, BackupMonitorClientStatus *clientStatusWindow, QObject *parent = 0);

    void paint(QPainter *painter,
          const QStyleOptionViewItem &option,
          const QModelIndex &index) const;
};

#endif // BACKUPMONITORCLIENTSTATUSDELEGATE_H
