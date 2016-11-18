#include "LoginWindow.h"
#include "ui_LoginWindow.h"

#include <TcpAbstractCommand.h>
#include <TcpCommands/main/connectTcp.h>
#include <TcpCommands/main/userLogin.h>

#include <MainWindow/MainWindow.h>
#include <LoginWindow/LoginManageServersWindow.h>

#include "TcpCommands/system/serverStatistics.h"

#include "LoginConnectionsManager.h"

#include <QList>
#include <QPair>
#include <QString>
#include <globals.h>
#include <QSettings>

LoginWindow::LoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    // Login- Button
    connect(ui->pushLogin, SIGNAL(clicked()), this, SLOT(doLogin()));

    // Wenn in den Eingabefeldern auf "Enter" gedrückt wird, so den Input Button auslösen:
    connect(ui->inputUsername, SIGNAL(returnPressed()), ui->pushLogin, SIGNAL(clicked()));
    connect(ui->inputPassword, SIGNAL(returnPressed()), ui->pushLogin, SIGNAL(clicked()));
    // @TODO: No such signal! Alternative suchen:
    //connect(ui->comboServer, SIGNAL(returnPressed()), ui->pushLogin, SIGNAL(clicked()));

    // Server verwalten:
    connect(ui->pushManageServers, SIGNAL(clicked()), this, SLOT(showManageServersWindow()));

    // Firmen- Logo setzen:
    ui->labelCompanyLogo->setPixmap( QPixmap(":/company/login_logo.jpg") );

    ui->labelClientVersion->setText(QString("Version: ") + VERSION);

    // Wenn keine auszuwählende Verbindung vorhanden ist, so erstellen wir eine Standard- Verbindung:
    if (LoginConnectionsManager::getConnectionsCount() < 1) {
        QString connId = LoginConnectionsManager::addConnection("SEP-Backupmonitor");
        LoginConnectionsManager::setHostname(connId, "sepbackupmonitor");
        LoginConnectionsManager::setPort(connId, "4500");
        LoginConnectionsManager::setStandardId(connId);
    }

    // Serververbindungen:
    initConnectionsList();

    ui->inputUsername->setFocus();

    setTabOrder(ui->inputUsername, ui->inputPassword);
    setTabOrder(ui->inputPassword, ui->comboConnections);
    setTabOrder(ui->comboConnections, ui->pushLogin);

    // Get last username:
    QSettings settings;
    QString lastUsername = settings.value("last_username").toString();
    if (lastUsername.isEmpty()) {
        ui->inputUsername->setFocus();
    }
    else {
        ui->inputUsername->setText(lastUsername);
        ui->inputPassword->setFocus();
    }
}

// ComboBox mit Verbindungen initialisieren:
void LoginWindow::initConnectionsList() {

    ui->comboConnections->clear();

    QList< QPair<QString, QString> > connections = LoginConnectionsManager::getAllConnectionNames();
    QPair<QString, QString> connection;
    QString standardId = LoginConnectionsManager::getStandardId();
    quint16 i = 0;
    quint16 standardItemId;
    foreach (connection, connections) {
        ui->comboConnections->addItem( connection.second, connection.first );
        if (connection.first == standardId) standardItemId = i;
        i++;
    }
    ui->comboConnections->setCurrentIndex( standardItemId );

}

/**
  * @slot
  * Login- Vorgang
  */
void LoginWindow::doLogin() {

    /**
      * Splash- Screen:
      */
    _splashScreen.setPixmap( QPixmap(":/splashScreen/background.png") );
    _splashScreen.show();

    _splashScreen.showMessage("Bitte warten...");


    this->hide();

    /**
      * Verbindung zum Server aufbauen:
      */
    _splashScreen.showMessage("Lese Verbindungskonfiguration aus...");
    QString connectionId = ui->comboConnections->itemData( ui->comboConnections->currentIndex() ).toString();
    QString hostname = LoginConnectionsManager::getHostname(connectionId);
    QString port = LoginConnectionsManager::getPort(connectionId);

    _splashScreen.showMessage( QString("Baue Verbindung zum TCP- Server %1, Port %2 auf...").arg(hostname, port) );;
    CREATE_TCP_COMMAND(tcp, main, connectTcp);
    tcp->setHostname( hostname );
    tcp->setPort( port.toInt() );

    tcp->startWait();

    // Konnte Verbindung aufgebaut werden??
    if (tcp->isError()) {
        loginError( QString("Connect to %1:%2: %3").arg(hostname, port ,tcp->errorString()) );
        return;
    }

    /**
      * Benutzer Authentifizierung:
      */
    _splashScreen.showMessage("Fuehre Benutzerauthentifizierung durch...");
    CREATE_TCP_COMMAND(userLogin, main, userLogin);
    userLogin->setUsername( ui->inputUsername->text() );
    userLogin->setPassword( ui->inputPassword->text() );

    userLogin->startWait();

    if (!userLogin->isSuccess()) {
        loginError( "Es wurde ein falscher Benutzername / Kennwort angegeben." );
        return;
    }

    QSettings settings;
    settings.setValue("last_username", ui->inputUsername->text());

    /**
      * Anwendung starten...
      */
    _splashScreen.showMessage("Starte Anwendung...");

    MainWindow *window = MainWindow::getInstance();
    _splashScreen.finish(window);
    window->show();

    close();
}

void LoginWindow::showManageServersWindow() {

    LoginManageServersWindow manageWindow;

    manageWindow.exec();

    // Nach schließen des Fensters die Verbindungs- Liste neu initialisieren:
    initConnectionsList();
}

void LoginWindow::loginError(const QString &text) {

    QMessageBox::critical(this, "Login Fehler", text);

    // Splash ausblenden & Login- Fenster einblenden...
    _splashScreen.hide();
    show();
}

LoginWindow::~LoginWindow()
{
    delete ui;
}
