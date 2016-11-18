#include "NotificationsWindow.h"
#include "ui_NotificationsWindow.h"

#include <QDateTime>
#include <QLabel>

NotificationsWindow::NotificationsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NotificationsWindow)
{
    ui->setupUi(this);

    _addMessage(QDateTime::currentDateTime(), "Das ist ein Test :)");
    _addMessage(QDateTime::currentDateTime(), "Das ist ein Test :)");
    _addMessage(QDateTime::currentDateTime(), "Das ist ein Test :)");
    _addMessage(QDateTime::currentDateTime(), "Das ist ein Test :)");
    _addMessage(QDateTime::currentDateTime(), "Das ist ein Test :)");
    _addMessage(QDateTime::currentDateTime(), "Das ist ein Test :)");
    _addMessage(QDateTime::currentDateTime(), "Das ist ein Test :)");
    _addMessage(QDateTime::currentDateTime(), "Das ist ein Test :)");
    _addMessage(QDateTime::currentDateTime(), "Das ist ein Test :)");
    _addMessage(QDateTime::currentDateTime(), "Das ist ein Test :)");
    _addMessage(QDateTime::currentDateTime(), "Das ist ein Test :)");
    _addMessage(QDateTime::currentDateTime(), "Das ist ein Test :)");
    _addMessage(QDateTime::currentDateTime(), "Das ist ein Test :)");
    _addMessage(QDateTime::currentDateTime(), "Das ist ein Test :)");
    _addMessage(QDateTime::currentDateTime(), "Das ist ein Test :)");
    _addMessage(QDateTime::currentDateTime(), "Das ist ein Test :)");
    _addMessage(QDateTime::currentDateTime(), "Das ist ein Test :)");
    _addMessage(QDateTime::currentDateTime(), "Das ist ein Test :)");
}

NotificationsWindow::~NotificationsWindow()
{
    delete ui;
}

void NotificationsWindow::_addMessage(const QDateTime &date, const QString &msg) {

    QHBoxLayout *layout = new QHBoxLayout;
    QLabel *labelDate = new QLabel(date.toString("dd.MM hh:mm:ss:"), this);
    QLabel *labelMessage = new QLabel(msg, this);

    labelDate->setStyleSheet("background-color: rgb(100, 100, 100)");
    labelMessage->setStyleSheet("background-color: rgb(100, 100, 100)");

    layout->addWidget(labelDate);
    layout->addWidget(labelMessage);
    layout->addStretch();

    ui->layoutChatMessages->addLayout(layout);
}
