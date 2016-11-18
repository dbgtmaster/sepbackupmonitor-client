#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QPushButton>
#include <QVBoxLayout>

#include <MainWindow/VersionWindow.h>
#include <MainWindow/TcpConnectionErrorDialog.h>
#include <MainWindow/TcpPermissionErrorWindow.h>

#include <Administraton/UserGroupsWindow.h>
#include <System/TcpCommandsPermissions/TcpCommandsPermissions.h>
#include <System/ServerPerformanceStatistics.h>

#include "TcpCommands/loggedInUser/hasTcpPermission.h"

#include <TcpMainThread.h>

#include <TcpCommands/loggedInUser/getUserColumn.h>

#include <BackupMonitor/BackupMonitorFactory.h>
#include "customerAdministration/CustomerAdministrationFactory.h"

MainWindow* MainWindow::_instance = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _mdiArea = new QMdiArea;
    setCentralWidget(_mdiArea);

    /**
      * Tcp Fehler behandeln...
      */
    connect(TcpMainThread::instance(), SIGNAL(tcpPermissionError(QString, QString)),
            this, SLOT(tcpPermissionErrorWindow(QString, QString)));

    _createTcpErrorSignalConnection();


    /**
      * Titel des Fensters...
      */
    CREATE_TCP_COMMAND(getDisplayName, loggedInUser, getUserColumn);
    getDisplayName->setColumn("displayname");
    getDisplayName->startWait();
    setWindowTitle(getDisplayName->getValue().toString() + " - Intranet - Siedl Networks GmbH");


    /**
      * Menüleiste:
      *
      * @TODO: Derzeit nicht möglich, ein erstelles Menü zu löschen, daher wird Menü erst erstellt, wenn es auch Unterpunkte erhält,
      * deshalb wird Performance- Statistik Punkt nur angezeigt, wenn auch System TCP befehle- berechtigungen angezeigt wird.
      */

    if (TcpCommands::hasTcpPermission("administration", "userGroupMod")) {

        QMenu* administration = menuBar()->addMenu("Administration");

        QAction *administration_usergroups = new QAction("Benutzergruppen", this);
        administration->addAction(administration_usergroups);
        connect(administration_usergroups, SIGNAL(triggered()), this, SLOT(administraton_usergroupsWindow()));
    }

    if (TcpCommands::hasTcpPermission("system", "saveTcpCommandsPermissions")) {
        QMenu* system = menuBar()->addMenu("System");

        QAction *system_tcpCommandsPermissions = new QAction("Tcp Befehle - Berechtigungen", this);
        system->addAction(system_tcpCommandsPermissions);
        connect(system_tcpCommandsPermissions, SIGNAL(triggered()), this, SLOT(system_tcpCommandsPermissionsWindow()));

        if (TcpCommands::hasTcpPermission("system", "serverStatistics")) {
            QAction *system_performanceStatistics = new QAction("Performance- Statistiken", this);
            system->addAction(system_performanceStatistics);
            connect(system_performanceStatistics, SIGNAL(triggered()), this, SLOT(system_performanceStatistics()));
        }
    }

    QMenu* help = menuBar()->addMenu("?");

    QAction *help_version = new QAction("Version", this);
    help->addAction(help_version);
    connect(help_version, SIGNAL(triggered()), this, SLOT(showVersion()));

    /**
      * Menü oben:
      */
    if (TcpCommands::hasTcpPermission("backupMonitor", "getJobs"))
        ui->dockMainMenuContents->addMenuEntry("Backup- Monitor", new BackupMonitorFactory);
    if (TcpCommands::hasTcpPermission("customers", "search"))
        ui->dockMainMenuContents->addMenuEntry("Kundenverwaltung", new CustomerAdministrationFactory);

    // Titelbar vom Menü löschen:
    delete ui->dockMainMenu->titleBarWidget();
    ui->dockMainMenu->setTitleBarWidget(new QWidget);

    setWindowState(Qt::WindowMaximized);

    QTimer::singleShot(1000, this, SLOT(init()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init() {
    ui->dockMainMenuContents->openMenuEntry("Backup- Monitor");
}

MainWindow* MainWindow::getInstance() {

    if (!_instance) {
        _instance = new MainWindow;
    }

    return _instance;
}

void MainWindow::_createTcpErrorSignalConnection() {

    connect(TcpMainThread::instance(), SIGNAL(tcpError(QAbstractSocket::SocketError,QString,QString)),
            this, SLOT(tcpErrorWindow(QAbstractSocket::SocketError,QString,QString)));
}

void MainWindow::addMdiSubWindow(QWidget *w) {

    _mdiArea->addSubWindow(w);
    w->setWindowState(Qt::WindowMaximized);
    w->show();
}

void MainWindow::showVersion() {

    VersionWindow *w = new VersionWindow;
    w->show();
}

void MainWindow::tcpErrorWindow(QAbstractSocket::SocketError, QString errMsg, QString) {

    disconnect(TcpMainThread::instance(), SIGNAL(tcpError(QAbstractSocket::SocketError,QString,QString)), 0, 0);

    TcpConnectionErrorDialog d;
    d.setErrorMessage(errMsg);
    d.exec();

    _createTcpErrorSignalConnection();
}

void MainWindow::tcpPermissionErrorWindow(QString groupName, QString actionName) {

    TcpPermissionErrorWindow w;
    w.setGroupName(groupName);
    w.setActionName(actionName);
    w.exec();
}

void MainWindow::tcpUnknownErrorWindow(QString) {

}

void MainWindow::administraton_usergroupsWindow() {

    UserGroupsWindow w;

    w.exec();
}

void MainWindow::system_tcpCommandsPermissionsWindow() {

    TcpCommandsPermissions w;

    w.exec();
}

void MainWindow::system_performanceStatistics() {

    ServerPerformanceStatistics w;

    w.exec();
}

void MainWindow::closeEvent(QCloseEvent *) {
    qApp->exit(0);
}
