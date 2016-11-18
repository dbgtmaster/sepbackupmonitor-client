#include "TcpConnectionErrorDialog.h"
#include "ui_TcpConnectionErrorDialog.h"

#include "TcpCommands/main/connectTcp.h"
#include "TcpCommands/main/userLogin.h"

#include <QTimer>

TcpConnectionErrorDialog::TcpConnectionErrorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TcpConnectionErrorDialog)
{
    ui->setupUi(this);

    connect(ui->pushExit, SIGNAL(clicked()), this, SLOT(closeApp()) );

    // Fenster schließen, maximieren etc.. ausblenden.
    setWindowFlags( Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint );

    QTimer::singleShot(0, this, SLOT(reconnect()));

    _closeApp = false;

    ui->progressBar->setMaximum(3);
    ui->progressBar->setValue(0);
}

TcpConnectionErrorDialog::~TcpConnectionErrorDialog()
{
    delete ui;
}

void TcpConnectionErrorDialog::reconnect() {


    if (_closeApp) {
        QCoreApplication::instance()->quit();
        return;
    }

    /**
      * Verbindung zum Server aufbauen:
      */
    ui->progressBar->setValue(1);
    ui->labelAction->setText("Verbinde zum Server...");
    CREATE_TCP_COMMAND(tcp, main, connectTcp);
    tcp->useLastHostData();
    tcp->startWait(false);

    // Konnte Verbindung aufgebaut werden??
    if (tcp->isError()) {
        ui->labelAction->setText(tcp->errorString() + "\nVersuche es in 2 Sekunden erneut...");
        QTimer::singleShot(2000, this, SLOT(reconnect()));
        return;
    }

    if (_closeApp) {
        QCoreApplication::instance()->quit();
        return;
    }

    /**
      * Benutzer Authentifizierung:
      */
    ui->progressBar->setValue(2);
    ui->labelAction->setText("Fuehre Benutzerauthentifizierung durch...");
    CREATE_TCP_COMMAND(userLogin, main, userLogin);
    userLogin->useSavedLoginData();
    userLogin->startWait(false);

    if (!userLogin->isSuccess()) {
        ui->labelAction->setText("Benutzerauthentifizierung fehlgeschlagen.\nVersuche es in 2 Sekunden erneut...");
        QTimer::singleShot(2000, this, SLOT(reconnect()));
        return;
        return;
    }


    if (_closeApp) {
        QCoreApplication::instance()->quit();
    }

    ui->progressBar->setValue(3);

    // Erfolgreich...
    close();
}

void TcpConnectionErrorDialog::closeApp() {
    _closeApp = true;
    ui->pushExit->setDisabled(true);
    ui->pushExit->setText( ui->pushExit->text() + " ... Bitte warten ..." );
}

void TcpConnectionErrorDialog::setErrorMessage(const QString &message) {

    ui->labelErrorMessage->setText(message);
}
