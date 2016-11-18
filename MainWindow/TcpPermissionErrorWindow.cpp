#include "TcpPermissionErrorWindow.h"
#include "ui_TcpPermissionErrorWindow.h"

TcpPermissionErrorWindow::TcpPermissionErrorWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TcpPermissionErrorWindow)
{
    ui->setupUi(this);

    connect(ui->pushOk, SIGNAL(clicked()), this, SLOT(close()));
}

TcpPermissionErrorWindow::~TcpPermissionErrorWindow()
{
    delete ui;
}

void TcpPermissionErrorWindow::setGroupName(const QString &groupName) {
    ui->labelGroupName->setText(groupName);
}

void TcpPermissionErrorWindow::setActionName(const QString &actionName) {
    ui->labelActionName->setText(actionName);
}
