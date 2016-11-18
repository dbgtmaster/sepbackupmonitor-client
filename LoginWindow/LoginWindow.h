#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QDebug>
#include <QAbstractSocket>
#include <QTimer>
#include <QSplashScreen>
#include <QMessageBox>

#include <TcpMainThread.h>

#include <TcpCommandsHelper.h>

namespace Ui {
    class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

private:
    // Screen, welcher während des Login- Vorgangs eingeblendet wird:
    QSplashScreen _splashScreen;

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

    // Initialisiert die Combobox mit der Liste der Verbindungen
    void initConnectionsList();

    // Wenn beim einloggen ein Fehler aufgetreten ist:
    void loginError(const QString& text);

public slots:
    void doLogin();

    void showManageServersWindow();

private:
    Ui::LoginWindow *ui;
};

#endif // LOGINWINDOW_H
