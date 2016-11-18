#include "BackupMonitorManageGroups.h"
#include "ui_BackupMonitorManageGroups.h"

#include <QMessageBox>

#include "TcpCommands/backupMonitor/getCustomerGroups.h"
#include "TcpCommands/backupMonitor/customerGroupMod.h"
#include "TcpCommands/backupMonitor/customerGroupDelete.h"

#include "BackupMonitor/BackupMonitorManageGroupsNew.h"

BackupMonitorManageGroups::BackupMonitorManageGroups(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BackupMonitorManageGroups), _groupsChanged(false)
{
    ui->setupUi(this);

    _disableEditGroup = false;

    connect(ui->pushNewGroup, SIGNAL(clicked()), this, SLOT(createNewGroup()));
    connect(ui->pushSave, SIGNAL(clicked()), this, SLOT(doSaveEditedGroup()));
    connect(ui->pushDelete, SIGNAL(clicked()), this, SLOT(deleteGroup()));

    connect(ui->pushUp, SIGNAL(clicked()), this, SLOT(groupUp()));
    connect(ui->pushDown, SIGNAL(clicked()), this, SLOT(groupDown()));

    // Wenn eine Gruppe Links im Menü ausgewählt wurde:
    connect(ui->listGroups, SIGNAL(itemSelectionChanged()), this, SLOT(startEditGroup()));

    connect(ui->widgetEditor, SIGNAL(changed()), this, SLOT(enableSaveButton()));

    ui->groupBoxEdit->setVisible(false);

    _reloadGroups();
    _refreshUpDownButtons();
}

BackupMonitorManageGroups::~BackupMonitorManageGroups()
{
    delete ui;
}

void BackupMonitorManageGroups::createNewGroup() {

    BackupMonitorManageGroupsNew newGroup;
    newGroup.exec();

    if (newGroup.isCreated()) {
        _reloadGroups();
        _groupsChanged = true;
    }
}

void BackupMonitorManageGroups::_refreshUpDownButtons() {

    // Ist ein Element ausgewählt??
    if (ui->listGroups->selectedItems().count() == 0) {
        ui->pushDown->setEnabled(false);
        ui->pushUp->setEnabled(false);
        return;
    }

    ui->pushDown->setEnabled(true);
    ui->pushUp->setEnabled(true);

    // Wenn die erste Gruppe ausgewählt wurde, so den UP Button deaktivieren:
    qDebug() << "aa" << ui->listGroups->currentRow();
    if (ui->listGroups->currentRow() == 0) {
        ui->pushUp->setEnabled(false);
    }

    // Wenn die letzte Gruppe ausgewählt wurde, so den DOWN Button deaktivieren:
    if (ui->listGroups->currentRow() == ui->listGroups->count() - 1) {
        ui->pushDown->setEnabled(false);
    }
}

void BackupMonitorManageGroups::_reloadGroups() {

    ui->listGroups->clear();

    // Wir lesen alle verfügbaren Gruppen aus:
    CREATE_TCP_COMMAND(getGroups, backupMonitor, getCustomerGroups);
    getGroups->setParameters(QStringList() << "id" << "name");
    getGroups->startWait();

    while(getGroups->hasNextCustomerGroup()) {
        QHash<QString, QVariant> group = getGroups->fetchNextCustomerGroup();

        QListWidgetItem *item = new QListWidgetItem(group.value("name").toString());
        item->setData(Qt::UserRole + 1, group.value("id").toInt());
        ui->listGroups->addItem( item );
    }
}

void BackupMonitorManageGroups::startEditGroup() {

    if (_disableEditGroup) return;

    // Ausgewählte ID:
    quint32 id = ui->listGroups->currentItem()->data(Qt::UserRole + 1).toInt();
    qDebug() << ui->listGroups->currentItem()->data(Qt::UserRole + 1);
    CREATE_TCP_COMMAND(getGroups, backupMonitor, getCustomerGroups);
    getGroups->setParameters(QStringList() << "id" << "name" << "auto_expand" << "description");
    getGroups->setFilterId(id);
    getGroups->startWait();

    if (!getGroups->hasNextCustomerGroup()) {
        QMessageBox::critical(this, "Error", trUtf8("Ausgewählte Gruppe existiert nicht!") );
        return;
    }
    QHash<QString, QVariant> data( getGroups->fetchNextCustomerGroup() );

    // Formular befüllen:
    ui->widgetEditor->setName(data["name"].toString());
    ui->widgetEditor->setDescription( data["description"].toString() );
    ui->widgetEditor->setAutoExpand( data["auto_expand"].toBool() );

    ui->groupBoxEdit->setVisible(true);
    ui->pushSave->setEnabled(false);        // Darf erst gesetzt werden, wenn alle Daten gesetzt wurden!

    _refreshUpDownButtons();
}

void BackupMonitorManageGroups::doSaveEditedGroup() {

    quint32 id = ui->listGroups->currentItem()->data(Qt::UserRole + 1).toInt();

    QHash<QString, QVariant> cols;
    cols["name"] = ui->widgetEditor->getName();
    cols["description"] = ui->widgetEditor->getDescription();
    cols["auto_expand"] = ui->widgetEditor->isAutoExpand();

    CREATE_TCP_COMMAND(mod, backupMonitor, customerGroupMod);
    mod->setAction(TcpCommand_backupMonitor_customerGroupMod::MODIFY);
    mod->setGroupId(id);
    mod->setCols(cols);
    mod->startWait();

    if (!mod->isSuccessful()) {
        // Es ist ein Fehler beim Bearbeiten aufgetreten, Benutzer informieren:
        QMessageBox::critical(this, "Fehler", mod->errorMessage());
        return;
    }

    // Links in der Gruppenliste den Namen anpassen, falls dieser geändert wurde.
    ui->listGroups->currentItem()->setText(ui->widgetEditor->getName());

    ui->pushSave->setEnabled(false);
    _groupsChanged = true;
}

void BackupMonitorManageGroups::enableSaveButton() {
    ui->pushSave->setEnabled(true);
}

void BackupMonitorManageGroups::deleteGroup() {

    int r = QMessageBox::question(this, "Gruppe löschen",
                          QString("Willst du die Gruppe '%1' wirklich löschen?").arg(ui->listGroups->currentItem()->text()),
                          QMessageBox::Yes, QMessageBox::No);

    if (r != QMessageBox::Yes) return;

    _disableEditGroup = true;

    quint32 id = ui->listGroups->currentItem()->data(Qt::UserRole + 1).toInt();

    CREATE_TCP_COMMAND(del, backupMonitor, customerGroupDelete);
    del->setGroupId(id);
    del->startWait();

    // Die gelöscht Gruppe aus der Liste entfernen
    delete ui->listGroups->takeItem( ui->listGroups->currentRow() );

    // Und keine Gruppe in der Liste auswählen...
    if (ui->listGroups->count() > 0) {
        ui->listGroups->currentItem()->setSelected(false);
    }
    ui->groupBoxEdit->setVisible(false);

    _disableEditGroup = false;

    _refreshUpDownButtons();
    _groupsChanged = true;
}

void BackupMonitorManageGroups::groupUp() {

    quint32 id = ui->listGroups->currentItem()->data(Qt::UserRole + 1).toInt();

    QHash<QString, QVariant> cols;
    cols["position"] = TcpCommand_backupMonitor_customerGroupMod::UP;

    CREATE_TCP_COMMAND(mod, backupMonitor, customerGroupMod);
    mod->setAction(TcpCommand_backupMonitor_customerGroupMod::MODIFY);
    mod->setGroupId(id);
    mod->setCols(cols);
    mod->startWait();

    if (!mod->isSuccessful()) {
        // Es ist ein Fehler beim Bearbeiten aufgetreten, Benutzer informieren:
        QMessageBox::critical(this, "Fehler", mod->errorMessage());
        return;
    }

    _disableEditGroup = true;
    int currentIndex = ui->listGroups->currentRow();
    QListWidgetItem *currentItem = ui->listGroups->takeItem(currentIndex);
    ui->listGroups->insertItem(currentIndex-1, currentItem);
    ui->listGroups->setCurrentRow(currentIndex-1);
    _disableEditGroup = false;

    _refreshUpDownButtons();
}

void BackupMonitorManageGroups::groupDown() {

    quint32 id = ui->listGroups->currentItem()->data(Qt::UserRole + 1).toInt();

    QHash<QString, QVariant> cols;
    cols["position"] = TcpCommand_backupMonitor_customerGroupMod::DOWN;

    CREATE_TCP_COMMAND(mod, backupMonitor, customerGroupMod);
    mod->setAction(TcpCommand_backupMonitor_customerGroupMod::MODIFY);
    mod->setGroupId(id);
    mod->setCols(cols);
    mod->startWait();

    if (!mod->isSuccessful()) {
        // Es ist ein Fehler beim Bearbeiten aufgetreten, Benutzer informieren:
        QMessageBox::critical(this, "Fehler", mod->errorMessage());
        return;
    }

    _disableEditGroup = true;
    int currentIndex = ui->listGroups->currentRow();
    QListWidgetItem *currentItem = ui->listGroups->takeItem(currentIndex);
    ui->listGroups->insertItem(currentIndex+1, currentItem);
    ui->listGroups->setCurrentRow(currentIndex+1);
    _disableEditGroup = false;

    _refreshUpDownButtons();
}
