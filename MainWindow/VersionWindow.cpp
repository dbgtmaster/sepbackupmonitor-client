#include "VersionWindow.h"
#include "ui_VersionWindow.h"

#include <TcpMainThread.h>
#include <TcpAbstractCommand.h>
#include <TcpCommands/main/serverVersion.h>

#include "globals.h"

#include <QTimer>

VersionWindow::VersionWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VersionWindow)
{
    ui->setupUi(this);

    ui->labelClientVersion->setText(VERSION);

    QTimer::singleShot(0, this, SLOT(getServerVersion()));
}

void VersionWindow::getServerVersion() {

    CREATE_TCP_COMMAND(version, main, serverVersion);
    version->startWait();

    ui->labelServerVersion->setText( version->getServerVersion() );
}

VersionWindow::~VersionWindow()
{
    delete ui;
}
