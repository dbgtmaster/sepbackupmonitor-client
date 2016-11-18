#include "BackupMonitorClientStatus.h"
#include "ui_BackupMonitorClientStatus.h"

#include <QDebug>
#include <QMenu>
#include <QTimer>
#include <QActionGroup>
#include <QMessageBox>

#include "BackupMonitor/BackupMonitorClientStatusItemModel.h"
#include "BackupMonitor/BackupMonitorClientStatusDelegate.h"
#include "BackupMonitor/BackupMonitorClientStatusItem.h"
#include "BackupMonitor/BackupMonitorClientStatusSortFilterProxy.h"
#include "BackupMonitor/BackupMonitorViewJob.h"
#include "BackupMonitor/BackupMonitorManageGroups.h"
#include "BackupMonitor/BackupMonitorExport.h"
#include "BackupMonitor/BackupMonitorLogWidget.h"

#include "customerAdministration/CustomerViewWindow.h"

#include "TcpCommands/customers/modCustomer.h"
#include "TcpCommands/customers/getCustomer.h"
#include "TcpCommands/backupMonitor/getCustomers.h"
#include "TcpCommands/backupMonitor/getCustomerGroups.h"

#include "Utils/UtilDateTime.h"

BackupMonitorClientStatus::BackupMonitorClientStatus(QWidget *parent, bool searchMode) :
    QWidget(parent), _searchModeEnabled(false),
    ui(new Ui::BackupMonitorClientStatus)
{
    ui->setupUi(this);

    /**
      * Bitte warten Widget
      */
    hidePleaseWait();
    _pleaseWaitWidget.setParent(this);

    connect(ui->pushManageGroups, SIGNAL(clicked()), this, SLOT(openManageGroupsWindow()));
    connect(ui->pushExport, SIGNAL(clicked()), this, SLOT(openExportWindow()));

    BackupMonitorClientStatusItemModel* model = new BackupMonitorClientStatusItemModel(ui->clientStatusTreeView, this);
    QSortFilterProxyModel *proxy = new QSortFilterProxyModel(this);
    //proxy->setSourceModel(model);
    ui->clientStatusTreeView->setModel(model);
    ui->clientStatusTreeView->setItemDelegate(new BackupMonitorClientStatusDelegate(ui->clientStatusTreeView, this, this));

    connect(model, SIGNAL(stateLoading(QString,int,int)), &_pleaseWaitWidget, SLOT(reloadState(QString,int,int)));

    ui->clientStatusTreeView->setMouseTracking(true);
    connect(ui->clientStatusTreeView, SIGNAL(entered(QModelIndex)),
             this, SLOT(clientStatusTreeViewEntered(QModelIndex)));

    ui->clientStatusTreeView->setContextMenuPolicy( Qt::CustomContextMenu );
    connect(ui->clientStatusTreeView, SIGNAL(customContextMenuRequested(QPoint)),
             this, SLOT(clientStatusContextMenuRequested(QPoint)));

    connect(ui->pushReloadSearchFilter, SIGNAL(clicked()),
            this, SLOT(reloadSearchFilter()));

    connect(ui->clientStatusTreeView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(jobDoubleClicked(QModelIndex)));

    connect(ui->pushReloadClientState, SIGNAL(clicked()),
            this, SLOT(reloadClientState()));

    enableSearchMode(searchMode);

    ui->clientStatusTreeView->setUniformRowHeights(false);

    if (!searchMode) {
        QTimer::singleShot(0, this, SLOT(initClientStatus()));
    }
    else {
        QTimer::singleShot(0, this, SLOT(initSearchMode()));
    }
}

BackupMonitorClientStatus::~BackupMonitorClientStatus()
{
    delete ui;
}

void BackupMonitorClientStatus::initClientStatus() {

    showPleaseWait();
        _reloadCustomerGroups();
        reloadClientState();
    hidePleaseWait();
}

void BackupMonitorClientStatus::initSearchMode() {

    // Kundenliste laden:
    showPleaseWait();
        _searchModeLoadCustomers();
    hidePleaseWait();
}

void BackupMonitorClientStatus::showPleaseWait() {

    _pleaseWaitWidget.move( ui->clientStatusTreeView->width() / 2 - _pleaseWaitWidget.width() / 2 ,
                      ui->clientStatusTreeView->height() / 2 - _pleaseWaitWidget.height() / 2);
    _pleaseWaitWidget.show();
}

void BackupMonitorClientStatus::hidePleaseWait() {

    _pleaseWaitWidget.hide();
}

void BackupMonitorClientStatus::showDateHeadline(bool t) {
    static_cast<BackupMonitorClientStatusItemModel*>(ui->clientStatusTreeView->model())
            ->_showDateHeadline = t;
}

void BackupMonitorClientStatus::reloadSearchFilter() {

    ui->pushReloadSearchFilter->setEnabled(false);
    showPleaseWait();

    BackupMonitorClientStatusItemModel *treeViewModel =
            static_cast<BackupMonitorClientStatusItemModel*>(ui->clientStatusTreeView->model());

    int customerId = ui->comboSearchFilterCustomers->itemData(
                ui->comboSearchFilterCustomers->currentIndex() ).toInt();

    int w0 = ui->clientStatusTreeView->columnWidth(0);
    int w1 = ui->clientStatusTreeView->columnWidth(1);
    int w2 = ui->clientStatusTreeView->columnWidth(2);
    int w3 = ui->clientStatusTreeView->columnWidth(3);
    int w4 = ui->clientStatusTreeView->columnWidth(4);
    treeViewModel->clear();

    treeViewModel->setHorizontalHeaderLabelsJobs();

    ui->clientStatusTreeView->setColumnWidth(0, w0);
    ui->clientStatusTreeView->setColumnWidth(1, w1);
    ui->clientStatusTreeView->setColumnWidth(2, w2);
    ui->clientStatusTreeView->setColumnWidth(3, w3);
    ui->clientStatusTreeView->setColumnWidth(4, w4);

    treeViewModel->reloadSearchFilter(customerId, ui->inputSearchFilterJobFilter->text(),
                                      ui->inputSearchFilterDateFrom->date(), ui->inputSearchFilterDateTo->date());

    hidePleaseWait();
    ui->pushReloadSearchFilter->setEnabled(true);
}

void BackupMonitorClientStatus::reloadClientState() {

    // Sortieren wird derzeit nur bei der Clientansicht unterstützt,
    // Da bei der "Durchsuchen" ansicht ansonsten die Abtrennugnen zwischen den Backup- Tagen durchgewürfelt werden..
    //ui->clientStatusTreeView->setSortingEnabled(true);
    //ui->clientStatusTreeView->sortByColumn(0, Qt::AscendingOrder);

    showPleaseWait();
    ui->pushReloadClientState->setEnabled(false);

    ui->labelHeader->setText( QString("Zeige: %1, %2")
                              .arg(UtilDateTime::getDayOfWeek(ui->inputClientStateDate->date()), ui->inputClientStateDate->date().toString("dd.MM.yyyy") ) );

    BackupMonitorClientStatusItemModel *treeViewModel =
            static_cast<BackupMonitorClientStatusItemModel*>(ui->clientStatusTreeView->model());

    int w0 = ui->clientStatusTreeView->columnWidth(0);
    int w1 = ui->clientStatusTreeView->columnWidth(1);
    int w2 = ui->clientStatusTreeView->columnWidth(2);
    int w3 = ui->clientStatusTreeView->columnWidth(3);
    int w4 = ui->clientStatusTreeView->columnWidth(4);
    int w5 = ui->clientStatusTreeView->columnWidth(5);
    int w6 = ui->clientStatusTreeView->columnWidth(6);
    treeViewModel->clear();

    treeViewModel->setHorizontalHeaderLabelsCustomers();

    ui->clientStatusTreeView->setColumnWidth(0, w0);
    ui->clientStatusTreeView->setColumnWidth(1, w1);
    ui->clientStatusTreeView->setColumnWidth(2, w2);
    ui->clientStatusTreeView->setColumnWidth(3, w3);
    ui->clientStatusTreeView->setColumnWidth(4, w4);
    ui->clientStatusTreeView->setColumnWidth(5, w5);
    ui->clientStatusTreeView->setColumnWidth(6, w6);

    // Damit "treeViewModel->clear()" an der Ausgabe auch sichtbar ist:
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

    ui->clientStatusTreeView->setUpdatesEnabled(false);

    treeViewModel->reloadClientStatus(ui->inputClientStateDate->date());

    //ui->clientStatusTreeView->setSortingEnabled(true);
    //ui->clientStatusTreeView->sortByColumn(0, Qt::AscendingOrder);


    ui->pushReloadClientState->setEnabled(true);
    ui->clientStatusTreeView->setUpdatesEnabled(true);
    hidePleaseWait();
}

void BackupMonitorClientStatus::enableSearchMode(bool t) {

    _searchModeEnabled = t;

    // Wenn Backup- Durchsuchen, so die Filter anzeigen:
    ui->groupBoxSearchFilter->setVisible(t);

    // Client Client- Status, so die Datumsanzeige anzeigen:
    ui->groupBoxClientStateView->setVisible(!t);

    BackupMonitorClientStatusItemModel *treeViewModel =
            static_cast<BackupMonitorClientStatusItemModel*>(ui->clientStatusTreeView->model());

    // Im Suchmodus wird in der Headel Line die Job- Leiste geschrieben,
    // ansonsten standardmässig die Kunden- Leiste
    if (t) {
        // Such- Modus:

        treeViewModel->setHorizontalHeaderLabelsJobs();

        // Von - Bis Datum setzen:
        ui->inputSearchFilterDateFrom->setDate( QDate::currentDate().addDays(-1) );
        ui->inputSearchFilterDateTo->setDate( QDate::currentDate().addDays(-1) );

        ui->clientStatusTreeView->setColumnWidth(0, 240);
        ui->clientStatusTreeView->setColumnWidth(1, 80);
        ui->clientStatusTreeView->setColumnWidth(2, 70);
        ui->clientStatusTreeView->setColumnWidth(3, 60);
        ui->clientStatusTreeView->setColumnWidth(4, 70);
        ui->clientStatusTreeView->setColumnWidth(5, 235);
    }
    else {
        // Client Status Modus:

        treeViewModel->setHorizontalHeaderLabelsCustomers();

        // Bei Datum heutiges Datum eintragen:
        ui->inputClientStateDate->setDate( QDate::currentDate().addDays(-1) );

        ui->clientStatusTreeView->setColumnWidth(0, 240);
        ui->clientStatusTreeView->setColumnWidth(1, 60);
        ui->clientStatusTreeView->setColumnWidth(2, 100);
        ui->clientStatusTreeView->setColumnWidth(3, 100);
        ui->clientStatusTreeView->setColumnWidth(4, 195);
        ui->clientStatusTreeView->setColumnWidth(5, 355);
        ui->clientStatusTreeView->setColumnWidth(6, 130);
    }
}

void BackupMonitorClientStatus::_searchModeLoadCustomers() {

    // Backup- Kunden ermitteln:
    CREATE_TCP_COMMAND(customerList, backupMonitor, getCustomers);
    customerList->startWait();

    ui->comboSearchFilterCustomers->clear();

    ui->comboSearchFilterCustomers->addItem("- Alle Kunden -", QVariant(-1) );

    QList< QPair<quint32, QString> > customers = customerList->getCustomerList();

    QPair<quint32, QString> customer;
    foreach(customer, customers) {

        ui->comboSearchFilterCustomers->addItem(customer.second, QVariant(customer.first) );
    }
}

void BackupMonitorClientStatus::clientStatusTreeViewEntered(QModelIndex index) {

    if (_searchModeEnabled) return;

    BackupMonitorClientStatusItem *item = static_cast<BackupMonitorClientStatusItem*>(
                static_cast<const BackupMonitorClientStatusItemModel*>(index.model())
                    ->itemFromIndex(index)
            );

    if (item->getItemType() == BackupMonitorClientStatusItem::MAIN_GROUP) {

        static_cast<BackupMonitorClientStatusItemModel*>(ui->clientStatusTreeView->model())
                    ->setHorizontalHeaderLabelsCustomers();
    }
    else {
        static_cast<BackupMonitorClientStatusItemModel*>(ui->clientStatusTreeView->model())
                    ->setHorizontalHeaderLabelsJobs();
    }
}

/**
  * Wenn im treeView auf einen Eintrag mit rechter Maustaste geklickt wird...
  */
void BackupMonitorClientStatus::clientStatusContextMenuRequested(QPoint pos) {

    if (!ui->clientStatusTreeView->currentIndex().isValid()) return;

    BackupMonitorClientStatusItem *item = static_cast<BackupMonitorClientStatusItem*>(
                static_cast<const BackupMonitorClientStatusItemModel*>(ui->clientStatusTreeView->currentIndex().model())
                    ->itemFromIndex(ui->clientStatusTreeView->currentIndex())
            );

    if (item->getItemType() == BackupMonitorClientStatusItem::HEADLINE) {
        return;
    }

    QMenu *menu = new QMenu(this);

    {
        // Jobs der letzten 25 Tage anzeigen
        // -------------------------------------------

        QAction *action = new QAction("Backup- Status der letzten 25 Tage", menu);
        menu->addAction(action);
        connect(action, SIGNAL(triggered()), this, SLOT(QActionShowJobs10DaysAgo()) );
        action->setData(item->getCustomerId());
    }

    if (item->getItemType() == BackupMonitorClientStatusItem::JOB) {

        // Diesen Job der letzten 25 Tage anzeigen
        // -------------------------------------------

        QAction *action = new QAction("Job Status der letzten 25 Tage", menu);
        menu->addAction(action);
        connect(action, SIGNAL(triggered()), this, SLOT(QActionShowJob10DaysAgo()) );
        QString taskName = item->parent()->child(item->row(), 0)->text();
        action->setData( QList<QVariant>() << item->getCustomerId() << taskName);
    }

    {
        QAction *seperator = new QAction(menu);
        seperator->setSeparator(true);
        menu->addAction(seperator);
    }

    if (item->getItemType() == BackupMonitorClientStatusItem::MAIN_GROUP)
    {

       // Kunden einer Gruppe zuordnen:
       // -------------------------------------------

        QMenu *menuGroupMembership = new QMenu( trUtf8("Gruppenzugehörigkeit") );
        menu->addMenu(menuGroupMembership);

        // Alle verfügbaren Gruppen auflisten:

        // Gruppe "Keine Zuordnung" temporär anlegen:
        QHash<QString, QVariant> noGroup;
        noGroup["id"] = 0;
        noGroup["name"] = "Keine Zuordnung";
        _customerGroups.append(noGroup);

        QList< QHash<QString, QVariant> >::ConstIterator it;
        QActionGroup *actionGroup = new QActionGroup(menu);
        actionGroup->setExclusive(true);
        for (it = _customerGroups.begin(); it != _customerGroups.end(); it++) {
            QVariant actionData;
            actionData.setValue( QList<QVariant>() << item->getCustomerId() << (*it).value("id").toInt() );
            QAction *actionGroupName = new QAction((*it).value("name").toString() , actionGroup);
            actionGroupName->setData( actionData );
            actionGroupName->setCheckable(true);
            if (static_cast<BackupMonitorClientStatusItem*>(item->parent())
                    ->getCustomerGroupId() == (*it).value("id").toInt()) {
                actionGroupName->setChecked(true);
            }
            menuGroupMembership->addAction(actionGroupName);
            actionGroup->addAction(actionGroupName);
            connect(actionGroupName, SIGNAL(triggered()), this, SLOT(QActionCustomerToGroupChanged()));
        }

        // Temporär angelegte Gruppe "Keine Zuordnung" wieder löschen...
        _customerGroups.takeLast();

        menuGroupMembership->addSeparator();

        QAction *actionManageGroups = new QAction("Gruppenverwaltung", menu);
        menuGroupMembership->addAction(actionManageGroups);
        connect(actionManageGroups, SIGNAL(triggered()), this, SLOT(openManageGroupsWindow()));
    }

    if (item->getItemType() != BackupMonitorClientStatusItem::CUSTOMER_GROUP) {
        // Kundeninformationen
        // -------------------------------------------

        QAction *action = new QAction("Kunden- Informationen", menu);
        menu->addAction(action);
        connect(action, SIGNAL(triggered()), this, SLOT(QActionShowCustomerWindow()) );

        action->setData(item->getCustomerId());

        // Log des Kunden anzeigen
        // -------------------------------------------

        {
            QAction *action = new QAction("Kunden- Log", menu);
            menu->addAction(action);
            connect(action, SIGNAL(triggered()), this, SLOT(QActionShowCustomerLogWindow()) );

            action->setData(item->getCustomerId());
        }
    }

    if (item->getItemType() != BackupMonitorClientStatusItem::MAIN_GROUP)
    {
        // Details eines Jobs betrachten:
        // -------------------------------------------

        QAction *action = new QAction("Job- Informationen", menu);
        menu->addAction(action);
        connect(action, SIGNAL(triggered()), this, SLOT(QActionShowJobWindow()) );
        action->setData(item->getJobId());
    }

    menu->exec( ui->clientStatusTreeView->mapToGlobal(pos) + QPoint(0, 25) );
    delete menu;
}

// Öffnet das Kunden- Informationsfenster:
void BackupMonitorClientStatus::QActionShowCustomerWindow() {

    int customerId = static_cast<QAction*>(sender())->data().toInt();

    CustomerViewWindow* view = new CustomerViewWindow(customerId);
    view->show();
}


// Öffnet das Kunden-Log Fenster:
void BackupMonitorClientStatus::QActionShowCustomerLogWindow() {

    int customerId = static_cast<QAction*>(sender())->data().toInt();

    // Get customername:
    // Nun ermitteln wir alle Spalten, welche wir von dem Kunden benötigen:
    QStringList columns;
    columns << "customer_name";
    CREATE_TCP_COMMAND(getCustomer, customers, getCustomer);
    getCustomer->setCustomerId(customerId);
    getCustomer->setColumns( columns );
    getCustomer->startWait();
    QHash<QString, QVariant> row = getCustomer->fetchRow();

    BackupMonitorLogWidget *view = new BackupMonitorLogWidget();
    view->setProperties(customerId, row["customer_name"].toString(), "");

    view->show();
}

// Öffnet Fenster für Informationen zu einem Job:
void BackupMonitorClientStatus::QActionShowJobWindow() {

    int jobId = static_cast<QAction*>(sender())->data().toInt();

    BackupMonitorViewJob *j = new BackupMonitorViewJob(jobId);
    j->show();
}

// Öffnet ein Fenster und zeigt die Jobs der letzten 10 Tage des Kunden:
void BackupMonitorClientStatus::QActionShowJobs10DaysAgo() {

    int customerId = static_cast<QAction*>(sender())->data().toInt();

    BackupMonitorClientStatus* win = new BackupMonitorClientStatus(0, true);

    // Datum von - bis setzen:
    win->ui->inputSearchFilterDateFrom->setDate( QDate::currentDate().addDays(-26) );
    win->ui->inputSearchFilterDateTo->setDate( QDate::currentDate().addDays(-1) );

    // KundenID setzen:
    win->ui->comboSearchFilterCustomers->clear();
    win->ui->comboSearchFilterCustomers->addItem("selectedCustomer", customerId);

    // Such- Funktion ausblenden:
    win->ui->groupBoxSearchFilter->setVisible(false);

    win->show();
    win->reloadSearchFilter();

    win->ui->clientStatusTreeView->expandAll();
}

void BackupMonitorClientStatus::QActionShowJob10DaysAgo() {

    QList<QVariant> data = static_cast<QAction*>(sender())->data().toList();
    int customerId = data.at(0).toInt();
    QString taskName = data.at(1).toString();

    BackupMonitorClientStatus* win = new BackupMonitorClientStatus(0, true);

    // Datum von - bis setzen:
    win->ui->inputSearchFilterDateFrom->setDate( QDate::currentDate().addDays(-26) );
    win->ui->inputSearchFilterDateTo->setDate( QDate::currentDate().addDays(-1) );

    // KundenID setzen:
    win->ui->comboSearchFilterCustomers->clear();
    win->ui->comboSearchFilterCustomers->addItem("selectedCustomer", customerId);

    // Namen des Jobs setzen:
    win->ui->inputSearchFilterJobFilter->setText(taskName);

    // Such- Funktion ausblenden:
    win->ui->groupBoxSearchFilter->setVisible(false);

    win->showDateHeadline(false);

    // Jobs nicht in Gruppen unterteilen:
    static_cast<BackupMonitorClientStatusItemModel*>(win->ui->clientStatusTreeView->model())
            ->setHideJobsGroups(true);

    win->show();
    win->reloadSearchFilter();

    win->ui->clientStatusTreeView->expandAll();
}

void BackupMonitorClientStatus::QActionCustomerToGroupChanged() {

    QList<QVariant> userData = static_cast<QAction*>(sender())->data().toList();
    quint32 customerId = userData[0].toInt();
    quint32 groupId = userData[1].toInt();

    QHash<QString, QVariant> cols;
    cols["backup_group"] = static_cast<int>(groupId);

    CREATE_TCP_COMMAND(mod, customers, modCustomer);
    mod->setCustomerId(customerId);
    mod->setModColumns(cols);
    mod->startWait();

    if (!mod->successfulModified()) {
        // Es ist ein Fehler beim Bearbeiten aufgetreten, Benutzer informieren:
        QMessageBox::critical(this, "Fehler", mod->getErrorMessage());
        return;
    }

    // Da das ändern der Gruppe erfolgreich war, schieben wir im Model den Benutzer in die neue Gruppe:
    static_cast<BackupMonitorClientStatusItemModel*>(ui->clientStatusTreeView->model())->moveCustomerToGroup(customerId, groupId);
}

void BackupMonitorClientStatus::jobDoubleClicked(QModelIndex index) {

    BackupMonitorClientStatusItem *item = static_cast<BackupMonitorClientStatusItem*>(
                static_cast<const BackupMonitorClientStatusItemModel*>(index.model())
                    ->itemFromIndex(index)
            );

    // Nur, wenn es ein Job ist...
    if (item->getItemType() != BackupMonitorClientStatusItem::JOB) return;

    BackupMonitorViewJob *j = new BackupMonitorViewJob( item->getJobId() );
    j->show();
}

void BackupMonitorClientStatus::openManageGroupsWindow() {
    BackupMonitorManageGroups *window = new BackupMonitorManageGroups;
    window->show();
}

void BackupMonitorClientStatus::openExportWindow() {
    BackupMonitorExport *window = new BackupMonitorExport;
    window->show();
}

void BackupMonitorClientStatus::_reloadCustomerGroups() {

    CREATE_TCP_COMMAND(groups, backupMonitor, getCustomerGroups);
    groups->setParameters(QStringList() << "id" << "name" << "auto_expand" << "description" << "position");
    groups->startWait();

    _customerGroups = groups->getAllCustomerGroups();
}
