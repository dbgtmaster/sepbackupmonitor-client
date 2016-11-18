#include "BackupMonitorClientStatusDelegate.h"

#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QLabel>

#include <Utils/UtilDateTime.h>

#include <QCoreApplication>

#include <QHeaderView>

#include "BackupMonitor/BackupMonitorClientStatusItem.h"
#include "BackupMonitor/BackupMonitorClientStatusItemModel.h"

BackupMonitorClientStatusDelegate::BackupMonitorClientStatusDelegate(QTreeView *view, BackupMonitorClientStatus *clientStatusWindow, QObject *parent) :
    QStyledItemDelegate(parent), _pixStateGreen(":/green.png"), _pixStateGreenInactive(":/green_inactive.png"),
    _pixStateYellow(":/yellow.png"), _pixStateYellowInactive(":/yellow_inactive.png"), _pixStateRed(":/red.png"), _pixStateRedInactive(":/red_inactive.png"),
    _pixStateBlue(":/blue.png"), _pixStateBlueInactive(":/blue_inactive.png"), _pixStatePurple(":/purple.png"), _pixStatePurpleInactive(":/purple_inactive.png"),
    _view(view), _clientStatusWindow(clientStatusWindow),
    _mousePopupClientStatusClicked(false)
{

    _mousePopupClientStatus = new BackupMonitorClientStatusMousePopup(view);
    _mousePopupClientStatus->hide();

    connect(view, SIGNAL(viewportEntered()), _mousePopupClientStatus, SLOT(doHide()));

    // Damit wir abfangen können, wenn Maus sich auf dem Header befindet:
    view->header()->installEventFilter(this);
}

void BackupMonitorClientStatusDelegate::paint(QPainter *painter,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const {

    BackupMonitorClientStatusItem* item =
            static_cast<BackupMonitorClientStatusItem*>(
                    static_cast<const BackupMonitorClientStatusItemModel*>(index.model())
                        ->itemFromIndex(index)
                );

    if (item == 0) {
        return QStyledItemDelegate::paint(painter, option, index);
    }

    if (item->getItem() == BackupMonitorClientStatusItem::ITEM_BACKUP_DAYS) {


        QHash<QString, QVariant> customer = item->getItemData().toHash();

        QStyledItemDelegate::paint(painter, option, index);

        int width = _pixStateGreen.width();
        int height = _pixStateGreen.height();
        int x = option.rect.x();
        int y = option.rect.y() + 2;

        painter->setPen(Qt::black);
        painter->setFont(QFont("Courier New", 8));

        if (customer["backup_prospective_jobs_mon"].toInt() > 0) {
            painter->setPen(Qt::black);
            painter->drawText(x, y + (option.rect.height() / 2) + 2, "MO");
        }
        else {
            painter->setPen(QColor(220, 220, 220));
            painter->drawText(x, y + (option.rect.height() / 2) + 2, "MO");
        }

        x = x + 17;

        if (customer["backup_prospective_jobs_tue"].toInt() > 0) {
            painter->setPen(Qt::black);
            painter->drawText(x, y + (option.rect.height() / 2) + 2, "DI");
        }
        else {
            painter->setPen(QColor(220, 220, 220));
            painter->drawText(x, y + (option.rect.height() / 2) + 2, "DI");
        }

        x = x + 17;

        if (customer["backup_prospective_jobs_wed"].toInt() > 0) {
            painter->setPen(Qt::black);
            painter->drawText(x, y + (option.rect.height() / 2) + 2, "MI");
        }
        else {
            painter->setPen(QColor(220, 220, 220));
            painter->drawText(x, y + (option.rect.height() / 2) + 2, "MI");
        }

        x = x + 17;

        if (customer["backup_prospective_jobs_thu"].toInt() > 0) {
            painter->setPen(Qt::black);
            painter->drawText(x, y + (option.rect.height() / 2) + 2, "DO");
        }
        else {
            painter->setPen(QColor(220, 220, 220));
            painter->drawText(x, y + (option.rect.height() / 2) + 2, "DO");
        }

        x = x + 17;

        if (customer["backup_prospective_jobs_fri"].toInt() > 0) {
            painter->setPen(Qt::black);
            painter->drawText(x, y + (option.rect.height() / 2) + 2, "FR");
        }
        else {
            painter->setPen(QColor(220, 220, 220));
            painter->drawText(x, y + (option.rect.height() / 2) + 2, "FR");
        }

        x = x + 17;

        if (customer["backup_prospective_jobs_sat"].toInt() > 0) {
            painter->setPen(Qt::black);
            painter->drawText(x, y + (option.rect.height() / 2) + 2, "SA");
        }
        else {
            painter->setPen(QColor(220, 220, 220));
            painter->drawText(x, y + (option.rect.height() / 2) + 2, "SA");
        }

        x = x + 17;

        if (customer["backup_prospective_jobs_sun"].toInt() > 0) {
            painter->setPen(Qt::black);
            painter->drawText(x, y + (option.rect.height() / 2) + 2, "SO");
        }
        else {
            painter->setPen(QColor(220, 220, 220));
            painter->drawText(x, y + (option.rect.height() / 2) + 2, "SO");
        }

        x = x + 17;

    }
    else if (item->getItem() == BackupMonitorClientStatusItem::ITEM_LAST_BACKUP_DAY) {

        int successful = item->data(Qt::UserRole + 2).toInt();
        int warning = item->data(Qt::UserRole + 3).toInt();
        int running = item->data(Qt::UserRole + 4).toInt();
        int error = item->data(Qt::UserRole + 5).toInt();
        int aborted = item->data(Qt::UserRole + 6).toInt();

        QStyledItemDelegate::paint(painter, option, index);

        int width = _pixStateGreen.width();
        int height = _pixStateGreen.height();
        int x = option.rect.x() + 28;
        int y = option.rect.y() + 2;

        painter->setFont(QFont("Arial", 8));

        if (successful > 0) {
            painter->drawPixmap(x, y, _pixStateGreen);
            painter->setPen(Qt::black);
        }
        else {
            painter->drawPixmap(x, y, _pixStateGreenInactive);
            painter->setPen(QColor(220, 220, 220));
        }
        x = x + width + 1;
        painter->drawText(x, y + (option.rect.height() / 2) + 2, QString::number(successful));
        x = x + 20;

        if (warning > 0) {
            painter->drawPixmap(x, y, _pixStateYellow);
            painter->setPen(Qt::black);
        }
        else {
            painter->drawPixmap(x, y, _pixStateYellowInactive);
            painter->setPen(QColor(220, 220, 220));
        }
        x = x + width + 1;
        painter->drawText(x, y + (option.rect.height() / 2 + 2), QString::number(warning));
        x = x + 20;

        if (error > 0) {
            painter->drawPixmap(x, y, _pixStateRed);
            painter->setPen(Qt::black);
        }
        else {
            painter->drawPixmap(x, y, _pixStateRedInactive);
            painter->setPen(QColor(220, 220, 220));
        }
        x = x + width + 1;
        painter->drawText(x, y + (option.rect.height() / 2) + 2, QString::number(error));
        x = x + 20;

        if (aborted > 0) {
            painter->drawPixmap(x, y, _pixStatePurple);
            painter->setPen(Qt::black);
        }
        else {
            painter->drawPixmap(x, y, _pixStatePurpleInactive);
            painter->setPen(QColor(220, 220, 220));
        }
        x = x + width + 1;
        painter->drawText(x, y + (option.rect.height() / 2) + 2, QString::number(aborted));
        x = x + 20;

        if (running > 0) {
            painter->drawPixmap(x, y, _pixStateBlue);
            painter->setPen(Qt::black);
        }
        else {
            painter->drawPixmap(x, y, _pixStateBlueInactive);
            painter->setPen(QColor(220, 220, 220));
        }
        x = x + width + 1;
        painter->drawText(x, y + (option.rect.height() / 2) + 2, QString::number(running));
    }
    else if (item->getItem() == BackupMonitorClientStatusItem::ITEM_RETROSPECTIVE_BACKUP) {

        QStyledItemDelegate::paint(painter, option, index);

        // Enthält einen String mit dem Format: (r|g|y|b) + 2 stellige Zahl.
        // zB.: g11g10y09y08g07g06
        // Beudetet soviel wie:
        // am 11. war Backup grün, am 10. gelb, am 09 & 08 auch gelb, 07 & 06 g.
        QString retrospectiveData = item->getItemData().toString();

        int x = option.rect.x();
        int y = option.rect.y() + 3;

        painter->setFont(QFont("Courier New", 8));

        // UtilDateTime::getShortDayOfWeek(tmpDate)

        int totalShow = item->data(Qt::UserRole).toInt();
        for (int i = 1; i <= totalShow; i++) {

            QString color = item->data(Qt::UserRole + 1 + (i * 7) + 0).toString();
            QDate date = item->data(Qt::UserRole + 1 +(i * 7) + 1).toDate();

            retrospectiveData.remove(0, 3);

            if (color == "g") {
                painter->setPen(QColor(37, 203, 7));
            }
            else if (color == "y") {
                painter->setPen(QColor(215, 209, 0));
            }
            else if (color == "b") {
                painter->setPen(QColor(20, 30, 190));
            }
            else if (color == "r") {
                painter->setPen(QColor(210, 0, 0));
            }

            painter->drawText(x, y + (option.rect.height() / 2), UtilDateTime::getShortDayOfWeek(date));
            x = x + 17;
        }
    }
    else {
        return QStyledItemDelegate::paint(painter, option, index);
    }
}

bool BackupMonitorClientStatusDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) {

    BackupMonitorClientStatusItem* item =
            static_cast<BackupMonitorClientStatusItem*>(
                    static_cast<const BackupMonitorClientStatusItemModel*>(index.model())
                        ->itemFromIndex(index) );

    /**
     * Abfragen, ob sich Maus gerade auf der Spalte der zurückblickenden Backups befindet.
     * Wenn ja, so anzeigen, so Popup anzeigen, wenn Maus über einen Tag fährt.
     * Bei Klick auf den Tag wird Popup dauerhaft angezeigt + einige Links (zeige diesen Tag,..)
     */
    if (event->type() == QEvent::MouseMove || event->type() == QEvent::MouseButtonPress) {

        if (item->getItem() == BackupMonitorClientStatusItem::ITEM_RETROSPECTIVE_BACKUP) {

            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

            int fieldStartX = option.rect.x();
            int mouseX = mouseEvent->x();
            int mouseY = mouseEvent->y();

            // In welcher Spalte befinden wir uns (eine Spalte ist 17px breit):
            int column = (mouseX - fieldStartX) / 17 + 1;
            int maxColumns = item->data(Qt::UserRole).toInt();  // Anzahl der Spalten, welche angezeigt werden
            if (column > maxColumns) {
                if (!_mousePopupClientStatusClicked) {
                    _mousePopupClientStatus->hide();
                }
            }
            else {

                // Popup wird nur geschoben, wenn nicht auf einem Tag geklickt wurde:
                if (!_mousePopupClientStatusClicked) {
                    QDate date = item->data(Qt::UserRole + 1 +(column * 7) + 1).toDate();
                    QString state = item->data(Qt::UserRole + 1 +(column * 7) + 2).toString();
                    int successful = item->data(Qt::UserRole + 1 +(column * 7) + 3).toInt();
                    int warning = item->data(Qt::UserRole + 1 +(column * 7) + 4).toInt();
                    int error = item->data(Qt::UserRole + 1 +(column * 7) + 5).toInt();
                    int abort = item->data(Qt::UserRole + 1 +(column * 7) + 6).toInt();

                    int headerHeight = _view->header()->size().height();
                    _mousePopupClientStatus->move(mouseX + 5, mouseY + headerHeight + 5);
                    _mousePopupClientStatus->setData(date, state, successful, warning, error, abort);
                    _mousePopupClientStatus->show();
                }

                // Es wurde mit der Maus auf einem Tag geklickt.
                /*if (event->type() == QEvent::MouseButtonPress) {
                    _mousePopupClientStatusClicked = true;
                    _mousePopupClientStatus->setMouseHoverMovePopup(false);
                }*/

            }
        }
        else {
            if (!_mousePopupClientStatusClicked) {
                _mousePopupClientStatus->hide();
            }
        }
    }

    // Wenn Maus die Anzeige der zurückblickende Backups verlässt:
    if (event->type() == QEvent::Leave && item->getItem() == BackupMonitorClientStatusItem::ITEM_RETROSPECTIVE_BACKUP) {
        if (!_mousePopupClientStatusClicked) {
            _mousePopupClientStatus->hide();
        }
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

bool BackupMonitorClientStatusDelegate::eventFilter(QObject *object, QEvent *event) {

    // Mauspopup ausblenden, wenn Maus über Headerline fährt...
    if (event->type() == QEvent::HoverEnter || event->type() == QEvent::Enter) {
        _mousePopupClientStatus->hide();
    }

    return true;
}
