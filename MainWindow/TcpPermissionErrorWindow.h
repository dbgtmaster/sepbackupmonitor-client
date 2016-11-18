#ifndef TCPPERMISSIONERRORWINDOW_H
#define TCPPERMISSIONERRORWINDOW_H

#include <QDialog>

namespace Ui {
class TcpPermissionErrorWindow;
}

class TcpPermissionErrorWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit TcpPermissionErrorWindow(QWidget *parent = 0);
    ~TcpPermissionErrorWindow();
    
    void setGroupName(const QString &groupName);
    void setActionName(const QString &actionName);

private:
    Ui::TcpPermissionErrorWindow *ui;
};

#endif // TCPPERMISSIONERRORWINDOW_H
