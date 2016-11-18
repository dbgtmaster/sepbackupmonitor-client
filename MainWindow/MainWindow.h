#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractSocket>
#include <QMdiArea>
#include <QSignalMapper>
#include <QMetaObject>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:

    static MainWindow *_instance;

    QMdiArea *_mdiArea;

protected:
    explicit MainWindow(QWidget *parent = 0);
    virtual void closeEvent(QCloseEvent *);

public:
    ~MainWindow();

    static MainWindow* getInstance();

    // Fügt ein neues Fenster hinzu...
    void addMdiSubWindow(QWidget *w);

private:

    void _createTcpErrorSignalConnection();

public slots:

    void init();

    void showVersion();

    // Zeigt Fenster an, dass ein Tcp- Fehler aufgetreten ist...
    void tcpErrorWindow(QAbstractSocket::SocketError, QString, QString);
    void tcpPermissionErrorWindow(QString groupName, QString actionName);
    void tcpUnknownErrorWindow(QString);

    // Öffnet Fenster für Berechtigungsverwaltung von Tcp- Befehlen...
    void system_tcpCommandsPermissionsWindow();
    void system_performanceStatistics();

    void administraton_usergroupsWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
