#include "LoginManageServersWindow.h"
#include "ui_LoginManageServersWindow.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>
#include <QStringList>
#include <QDebug>
#include <QPair>
#include <QListWidget>

#include "LoginConnectionsManager.h"

LoginManageServersWindow::LoginManageServersWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginManageServersWindow)
{
    ui->setupUi(this);

    connect(ui->pushAddServer, SIGNAL(clicked()), this, SLOT(addConnectionDialog()));
    connect(ui->pushDeleteConnection, SIGNAL(clicked()), this, SLOT(deleteConnectionDialog()));

    connect(ui->listConnections, SIGNAL(itemSelectionChanged()), this, SLOT(showConnectionProperties()));

    connect(ui->pushSave, SIGNAL(clicked()), this, SLOT(saveConnectionProperties()));
    connect(ui->pushCancel, SIGNAL(clicked()), this, SLOT(close()));

    ui->widgetEditConnection->hide();

    reloadConnectionsList();
}

void LoginManageServersWindow::reloadConnectionsList(QString selectId) {

    QList< QPair<QString, QString> > list = LoginConnectionsManager::getAllConnectionNames();
    QString standardId = LoginConnectionsManager::getStandardId();

    // Und ab mit den Namen in die Liste:
    ui->listConnections->clear();
    QPair<QString, QString> pair;
    foreach(pair, list) {
        QListWidgetItem* item = new QListWidgetItem(ui->listConnections);

        if (pair.first == standardId) {
            item->setText( QString("%1 [*]").arg(pair.second) );
        }
        else {
            item->setText(pair.second);
        }

        item->setData(Qt::UserRole, QVariant(pair.first) );
        ui->listConnections->addItem(item);

        // Element auswählen??
        if (pair.first == selectId) {
            item->setSelected(true);
        }
    }
}

void LoginManageServersWindow::addConnectionDialog() {

    bool ok;
    QString connectionName = QInputDialog::getText(this, "Neue Verbindung anlegen", "Bitte geben Sie einen Namen für die neue Verbindung an:", QLineEdit::Normal, QString(), &ok);

    if (!ok) return;

    quint16 createdId = LoginConnectionsManager::addConnection(connectionName).toInt();
    if (!createdId) {

        QMessageBox::critical(this, "Fehler", "Verbindung konnte nicht angelegt werden");
        return;
    }

    reloadConnectionsList( QString::number(createdId) );

    // Und nun die neu erstellte Verbindung auswählen:
    //ui->listConnections->
}

void LoginManageServersWindow::deleteConnectionDialog() {

    // Wenn nichts ausgewählt wurde...
    if (ui->listConnections->selectedItems().count() < 1) return;

    QListWidgetItem* selectItem = ui->listConnections->selectedItems().first();

    QString text = selectItem->text();
    QString id = selectItem->data(Qt::UserRole).toString();

    int r = QMessageBox::warning(this, "Verbindung löschen", QString("Soll die Verbindung '%1' wirklich gelöscht werden?").arg(text),
                                  QMessageBox::Yes, QMessageBox::No);

    if (r != QMessageBox::Yes) return;

    LoginConnectionsManager::deleteConnection(id);

    reloadConnectionsList();
    ui->widgetEditConnection->hide();
}

void LoginManageServersWindow::showConnectionProperties() {

    if (ui->listConnections->selectedItems().count() < 1) return;

    QListWidgetItem* selectItem = ui->listConnections->selectedItems().first();

    ui->widgetEditConnection->show();

    _selectedId = selectItem->data(Qt::UserRole).toString();

    ui->inputName->setText( LoginConnectionsManager::getName(_selectedId) );
    ui->inputHostname->setText( LoginConnectionsManager::getHostname(_selectedId) );
    ui->inputPort->setText( LoginConnectionsManager::getPort(_selectedId) );

    ui->checkboxStandard->setChecked( (_selectedId == LoginConnectionsManager::getStandardId()) );
}

void LoginManageServersWindow::saveConnectionProperties() {

    LoginConnectionsManager::setName(_selectedId, ui->inputName->text());
    LoginConnectionsManager::setHostname(_selectedId, ui->inputHostname->text());
    LoginConnectionsManager::setPort(_selectedId, ui->inputPort->text());

    // Wurde Element als Standard gewählt??
    if (ui->checkboxStandard->checkState() == Qt::Checked) {
        LoginConnectionsManager::setStandardId(_selectedId);
    }

    reloadConnectionsList(_selectedId);

    QMessageBox::information(this, "Saved", "Settings saved");
}

LoginManageServersWindow::~LoginManageServersWindow()
{
    delete ui;
}
