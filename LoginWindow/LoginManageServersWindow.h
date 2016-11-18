#ifndef LOGINMANAGESERVERSWINDOW_H
#define LOGINMANAGESERVERSWINDOW_H

#include <QDialog>
#include <QModelIndex>

class QString;
class QListWidgetItem;

namespace Ui {
    class LoginManageServersWindow;
}

class LoginManageServersWindow : public QDialog
{
    Q_OBJECT

private:

    QString _selectedId;

    // Ladet die Liste mit allen Verfügbaren Verbindungen neu:
    // _selectId: ID, welche vor- ausgewählt werden soll...
    void reloadConnectionsList(QString selectId = QString());
public:
    explicit LoginManageServersWindow(QWidget *parent = 0);
    ~LoginManageServersWindow();

    bool addConnection(const QString &name);

private slots:
    void addConnectionDialog();
    void deleteConnectionDialog();

    void showConnectionProperties();
    void saveConnectionProperties();

private:
    Ui::LoginManageServersWindow *ui;
};

#endif // LOGINMANAGESERVERSWINDOW_H
