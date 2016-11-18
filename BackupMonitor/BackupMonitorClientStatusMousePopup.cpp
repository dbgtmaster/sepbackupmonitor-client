#include "BackupMonitorClientStatusMousePopup.h"
#include "ui_BackupMonitorClientStatusMousePopup.h"

#include <QDate>
#include <QMouseEvent>
#include <QDebug>
#include <QObject>

BackupMonitorClientStatusMousePopup::BackupMonitorClientStatusMousePopup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BackupMonitorClientStatusMousePopup), _mouseHoverMovePopup(true)
{
    ui->setupUi(this);

    setMouseTracking(true);

    /**
     * Wir müssen das Maus-Tracking auf alle Childs aktivieren, damit wir die Position des Mauszeigers ermitteln könne..
     */
    QObjectList pList = children();
    QWidget *widget;
    for (QObjectList::Iterator i = pList.begin(); i != pList.end(); i++) {
        if (widget = dynamic_cast<QWidget*>((*i))) {
            widget->setMouseTracking(true);
         }
    }
}

BackupMonitorClientStatusMousePopup::~BackupMonitorClientStatusMousePopup()
{
    delete ui;
}

void BackupMonitorClientStatusMousePopup::setData(QDate &date, QString &message, int successful, int warning, int error, int abort) {

    ui->labelDate->setText(date.toString());
    ui->labelState->setText(message);
    ui->labelSuccessful->setText(QString("%1 erfolgreich").arg(QString::number(successful)));
    ui->labelWarning->setText(QString("%1 mit Warnungen").arg(QString::number(warning)));
    ui->labelError->setText(QString("%1 fehlerhaft").arg(QString::number(error)));
    ui->labelAborted->setText(QString("%1 abgebrochen").arg(QString::number(abort)));
}

void BackupMonitorClientStatusMousePopup::doHide() {
    hide();
}

void BackupMonitorClientStatusMousePopup::mouseMoveEvent(QMouseEvent *event) {

    if (_mouseHoverMovePopup) {
        QPoint p = event->pos();
        move( x() + p.x(), y() + p.y() );
    }
}
