#include "UserGroupCreatorWindow.h"
#include "ui_UserGroupCreatorWindow.h"

#include <QMessageBox>

#include "TcpCommands/administration/userGroupMod.h"
#include "TcpCommands/administration/userGroupToSystemGroups.h"

UserGroupCreatorWindow::UserGroupCreatorWindow(QWidget *parent) :
    QDialog(parent), _groupCreated(false),
    ui(new Ui::UserGroupCreatorWindow)
{
    ui->setupUi(this);

    connect(ui->pushCancel, SIGNAL(clicked()), this, SLOT(close()));

    ui->widgetGroupEditor->hideGroupDeleteButton();
}

UserGroupCreatorWindow::~UserGroupCreatorWindow()
{
    delete ui;
}

// Benutzergruppe erstellen:
void UserGroupCreatorWindow::on_pushCreate_clicked() {

    CREATE_TCP_COMMAND(createGroup, administration, userGroupMod);

    createGroup->setAction( TcpCommand_administration_userGroupMod::CREATE );
    createGroup->setGroupName( ui->widgetGroupEditor->getGroupName() );
    createGroup->setIsLocked( ui->widgetGroupEditor->isLocked() );
    createGroup->setLdapFilter1( ui->widgetGroupEditor->getLdapFilter1() );
    createGroup->setLdapFilter2( ui->widgetGroupEditor->getLdapFilter2() );
    createGroup->setLdapFilter3( ui->widgetGroupEditor->getLdapFilter3() );

    createGroup->setLdapFilter1Enabled( ui->widgetGroupEditor->isLdapFilter1Enabled() );
    createGroup->setLdapFilter2Enabled( ui->widgetGroupEditor->isLdapFilter2Enabled() );
    createGroup->setLdapFilter3Enabled( ui->widgetGroupEditor->isLdapFilter3Enabled() );

    createGroup->startWait();

    // Konnte Gruppe angelegt werden??
    if (!createGroup->isSuccess()) {

        QMessageBox::critical(this, "Ein Fehler ist aufgetreten",
                              QString("Gruppe konnte nicht angelegt werden:<br/>%1").arg(createGroup->getErrorMessage()) );

        return;
    }

    /**
      * Gruppen zu Systemgruppen zuordnung:
      */
    CREATE_TCP_COMMAND(groups, administration, userGroupToSystemGroups);
    groups->setGroupId( createGroup->getId() );
    groups->setMemberships( ui->widgetGroupEditor->getChoosedSystemGroups() );
    groups->startWait();

    _groupCreated = true;
    close();
}
