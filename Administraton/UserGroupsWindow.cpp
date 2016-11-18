#include "UserGroupsWindow.h"
#include "ui_UserGroupsWindow.h"

#include "UserGroupCreatorWindow.h"

#include "TcpCommands/administration/getAllUserGroups.h"
#include "TcpCommands/administration/getUserGroup.h"
#include "TcpCommands/administration/userGroupMod.h"
#include "TcpCommands/administration/userGroupDelete.h"
#include "TcpCommands/administration/userGroupToSystemGroups.h"

#include <QMessageBox>

UserGroupsWindow::UserGroupsWindow(QWidget *parent) :
    QDialog(parent), _disableEditGroup(false),
    ui(new Ui::UserGroupsWindow)
{
    ui->setupUi(this);

    connect(ui->pushCreateGroup, SIGNAL(clicked()), this, SLOT(createNewGroupWindow()));
    connect(ui->pushCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->pushOk, SIGNAL(clicked()), this, SLOT(ok()));
    connect(ui->pushApply, SIGNAL(clicked()), this, SLOT(apply()));
    connect(ui->widgetEditor, SIGNAL(groupDeleteClicked()), this, SLOT(remove()));

    // Wenn eine Gruppe ausgewählt wird...
    connect(ui->listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(startEdit()));

    // Wenn ein Wert geändert wird, so den Übernehmen Button aktiv schalten:
    connect(ui->widgetEditor, SIGNAL(valueChanged(bool)), ui->pushApply, SLOT(setEnabled(bool)));

    hideEditor();

    _reloadUserGroups();
}

UserGroupsWindow::~UserGroupsWindow()
{
    delete ui;
}

void UserGroupsWindow::_reloadUserGroups() {

    ui->listWidget->clear();

    /**
      * Alle verfügbaren Bentuzergruppen lesen:
      */
    CREATE_TCP_COMMAND(getGroups, administration, getAllUserGroups);
    getGroups->setColumns( QStringList() << "id" << "name" );
    getGroups->startWait();

    while(getGroups->hasNext()) {

        QListWidgetItem *item = new QListWidgetItem(getGroups->getColumn("name").toString());
        item->setData(Qt::UserRole + 1, getGroups->getColumn("id").toInt());
        ui->listWidget->addItem( item );
    }
}

void UserGroupsWindow::showEditor() {

    ui->widgetEditor->show();
    ui->pushCancel->show();
    ui->pushOk->show();
    ui->pushApply->show();

    ui->pushApply->setEnabled(false);
}

void UserGroupsWindow::hideEditor() {

    ui->widgetEditor->hide();
    ui->pushCancel->hide();
    ui->pushOk->hide();
    ui->pushApply->hide();
}

void UserGroupsWindow::createNewGroupWindow() {

    UserGroupCreatorWindow w;
    w.exec();

    // Wenn eine Gruppe erstellt wurde, so Gruppenliste neu laden:
    if (w.groupCreated()) {
        _reloadUserGroups();
    }
}

void UserGroupsWindow::startEdit() {

    if (_disableEditGroup) return;

    /**
      * Ausgewählte Gruppe bearbeiten:
      */

    CREATE_TCP_COMMAND(getGroup, administration, getUserGroup);

    getGroup->setId( ui->listWidget->currentItem()->data(Qt::UserRole + 1).toInt() );
    getGroup->setColumns(
                QStringList() << "id" << "name" << "locked"
                << "ldapfilter1_enabled" << "ldapfilter2_enabled" << "ldapfilter3_enabled"
                << "ldapfilter1_scope" << "ldapfilter2_scope" << "ldapfilter3_scope"
                << "ldapfilter1_dn" << "ldapfilter2_dn" << "ldapfilter3_dn"
                << "ldapfilter1_filter" << "ldapfilter2_filter" << "ldapfilter3_filter"
                );

    getGroup->setGetSystemMemberships(true);

    getGroup->startWait();

    if (!getGroup->exists()) {
        QMessageBox::critical(this, "Fehler", "Ausgewählte Gruppe existiert nicht mehr!");
        delete ui->listWidget->takeItem( ui->listWidget->currentRow() );
        return;
    }

    ui->widgetEditor->setGroupName( getGroup->getColumn("name").toString() );
    ui->widgetEditor->setIsLocked( getGroup->getColumn("locked").toBool() );

    ui->widgetEditor->setLdapFilter1( getGroup->getLdapFilter1() );
    ui->widgetEditor->setLdapFilter2( getGroup->getLdapFilter2() );
    ui->widgetEditor->setLdapFilter3( getGroup->getLdapFilter3() );
    ui->widgetEditor->setIsLdapFilter1Enabled( getGroup->getColumn("ldapfilter1_enabled").toBool() );
    ui->widgetEditor->setIsLdapFilter2Enabled( getGroup->getColumn("ldapfilter2_enabled").toBool() );
    ui->widgetEditor->setIsLdapFilter3Enabled( getGroup->getColumn("ldapfilter3_enabled").toBool() );

    // Mitgliedschaften zu den Systemgruppen:
    ui->widgetEditor->setChoosedSystemGroups( getGroup->getSystemGroupsMembership() );

    showEditor();

}

void UserGroupsWindow::ok() {

    apply();
    close();
}

void UserGroupsWindow::apply() {

    // Speichern nur möglich, wenn der "Übernehmen" Button nicht inaktiv ist...
    if (!ui->pushApply->isEnabled()) return;

    /**
      * Einstellungen der Gruppe speichern:
      */

    int id = ui->listWidget->currentItem()->data(Qt::UserRole + 1).toInt();

    CREATE_TCP_COMMAND(mod, administration, userGroupMod);

    mod->setAction(TcpCommand_administration_userGroupMod::MODIFY);
    mod->setId( id );

    mod->setGroupName(ui->widgetEditor->getGroupName());
    mod->setIsLocked(ui->widgetEditor->isLocked());

    mod->setLdapFilter1(ui->widgetEditor->getLdapFilter1());
    mod->setLdapFilter2(ui->widgetEditor->getLdapFilter2());
    mod->setLdapFilter3(ui->widgetEditor->getLdapFilter3());
    mod->setLdapFilter1Enabled(ui->widgetEditor->isLdapFilter1Enabled());
    mod->setLdapFilter2Enabled(ui->widgetEditor->isLdapFilter2Enabled());
    mod->setLdapFilter3Enabled(ui->widgetEditor->isLdapFilter3Enabled());

    mod->startWait();

    if (mod->isSuccess()) {

        /**
          * Zuordnung zu den Systemgruppen speichern:
          */
        CREATE_TCP_COMMAND(modMembership, administration, userGroupToSystemGroups);
        modMembership->setGroupId(id);
        modMembership->setMemberships( ui->widgetEditor->getChoosedSystemGroups() );
        modMembership->startWait();

        ui->pushApply->setEnabled(false);
    }
    else {
        QMessageBox::critical(this, "Ein Fehler ist aufgetreten",
                              QString("Gruppe konnte nicht bearbeitet werden:<br/>%1").arg(mod->getErrorMessage()) );

        return;
    }
}

void UserGroupsWindow::remove() {

    int r = QMessageBox::question(this, "Gruppe löschen",
                          QString("Willst du die Gruppe '%1' wirklich löschen?").arg(ui->listWidget->currentItem()->text()),
                          QMessageBox::Yes, QMessageBox::No);

    if (r != QMessageBox::Yes) return;

    QListWidgetItem* currentItem =  ui->listWidget->currentItem();
    int row = ui->listWidget->currentRow();
    quint32 id = currentItem->data(Qt::UserRole + 1).toInt();

    CREATE_TCP_COMMAND(del, administration, userGroupDelete);

    del->setId( id );
    del->startWait();

    if (!del->successful()) {
        QMessageBox::critical(this, "Fehler", "Benutzergruppe konnte nicht gelöscht werden");
        return;
    }

    //QMessageBox::information(this, "Gruppe gelöscht", QString("Gruppe '%1' wurde erfolgreich gelöscht.").arg(ui->listWidget->currentItem()->text()) );

    _disableEditGroup = true;

    delete ui->listWidget->takeItem( row );

    ui->listWidget->clearSelection();
    ui->listWidget->clearFocus();

    _disableEditGroup = false;

    hideEditor();
}
