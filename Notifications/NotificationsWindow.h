#ifndef NOTIFICATIONSWINDOW_H
#define NOTIFICATIONSWINDOW_H

#include <QWidget>

class QDateTime;

namespace Ui {
class NotificationsWindow;
}

class NotificationsWindow : public QWidget
{
    Q_OBJECT
    
    // Fügt eine neue Nachricht hinzu:
    void _addMessage(const QDateTime &date, const QString &msg);

public:
    explicit NotificationsWindow(QWidget *parent = 0);
    ~NotificationsWindow();
    
private:
    Ui::NotificationsWindow *ui;
};

#endif // NOTIFICATIONSWINDOW_H
