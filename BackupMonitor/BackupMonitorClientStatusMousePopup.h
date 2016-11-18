#ifndef BACKUPMONITORCLIENTSTATUSMOUSEPOPUP_H
#define BACKUPMONITORCLIENTSTATUSMOUSEPOPUP_H

#include <QWidget>

class QDate;

namespace Ui {
class BackupMonitorClientStatusMousePopup;
}

class BackupMonitorClientStatusMousePopup : public QWidget
{
    Q_OBJECT

    // Soll das Popup geschoben werden, wenn sich die Maus darauf befindet?
    bool _mouseHoverMovePopup;
    
public:
    explicit BackupMonitorClientStatusMousePopup(QWidget *parent = 0);
    ~BackupMonitorClientStatusMousePopup();
    
    void setData(QDate &date, QString &message, int successful, int warning, int error, int abort);

    inline void setMouseHoverMovePopup(bool t) {
        _mouseHoverMovePopup = t;
    }

    void mouseMoveEvent(QMouseEvent *);
public slots:
    void doHide();

private:
    Ui::BackupMonitorClientStatusMousePopup *ui;
};

#endif // BACKUPMONITORCLIENTSTATUSMOUSEPOPUP_H
