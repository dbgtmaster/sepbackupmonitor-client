#include "BackupMonitor/BackupMonitorListJobs.h"

#include "ui_BackupMonitorListJobs.h"

#include <QDate>
#include <QMenu>

#include "TcpCommands/backupMonitor/getCustomers.h"
#include "TcpCommands/customers/getCustomer.h"

#include "BackupMonitor/BackupMonitorViewJob.h"

#include "BackupMonitor/BackupMonitorDelegate.h"
#include "BackupMonitor/BackupMonitorItem.h"

BackupMonitorListJobs::BackupMonitorListJobs(bool loadCustomerList, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BackupMonitorListJobs)
{
    ui->setupUi(this);

    ui->inputDateViewDateFrom->setDate( QDate::currentDate().addDays(-1) );
    ui->inputDateViewDateTo->setDate( QDate::currentDate().addDays(-1) );

    ui->inputCustomerViewDateFrom->setDate( QDate::currentDate().addDays(-5) );
    ui->inputCustomerViewDateTo->setDate( QDate::currentDate().addDays(-1) );

    // Die einzelnen Ansichten:
    connect(ui->radioViewCustomer, SIGNAL(toggled(bool)), ui->groupBoxFilterCustomer, SLOT(setVisible(bool)));
    connect(ui->radioViewDate, SIGNAL(toggled(bool)), ui->groupBoxFilterDate, SLOT(setVisible(bool)));
    ui->groupBoxFilterCustomer->setVisible(false);

    _backupModel = new BackupMonitorItemModel;
    ui->backupView->setModel( _backupModel );
    ui->backupView->setColumnWidth(0, 240);
    ui->backupView->setColumnWidth(1, 140);
    ui->backupView->setColumnWidth(2, 70);
    ui->backupView->setColumnWidth(3, 70);
    //ui->backupView->setColumnWidth(3, 100);

    ui->backupView->setContextMenuPolicy( Qt::CustomContextMenu );
    ui->backupView->setItemDelegate( new BackupMonitorDelegate );

    if (loadCustomerList) {
        _reloadCustomerList();
    }

    connect(ui->pushReload, SIGNAL(clicked()), this, SLOT(reloadBackupMonitor()));

    connect(ui->backupView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(jobClicked(QModelIndex)));
    connect(ui->backupView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequested(QPoint)));
}

BackupMonitorListJobs::~BackupMonitorListJobs()
{
    delete ui;
}

void BackupMonitorListJobs::_reloadCustomerList() {

    // Backup- Kunden ermitteln:
    CREATE_TCP_COMMAND(customerList, backupMonitor, getCustomers);
    customerList->startWait();

    QList< QPair<quint32, QString> > customers = customerList->getCustomerList();

    QPair<quint32, QString> customer;
    foreach(customer, customers) {

        ui->comboCustomers->addItem(customer.second, QVariant(customer.first) );
    }
}

void BackupMonitorListJobs::jobClicked(QModelIndex index) {

    BackupMonitorItem *item = static_cast<BackupMonitorItem*>(
                static_cast<const BackupMonitorItemModel*>(index.model())
                    ->itemFromIndex(index)
            );

    BackupMonitorViewJob *j = new BackupMonitorViewJob( item->getJobId() );
    j->show();
}

void BackupMonitorListJobs::contextMenuRequested(QPoint pos) {

    if (!ui->backupView->currentIndex().isValid()) return;

    BackupMonitorItem *item = static_cast<BackupMonitorItem*>(
                static_cast<const BackupMonitorItemModel*>(ui->backupView->currentIndex().model())
                    ->itemFromIndex(ui->backupView->currentIndex())
            );

    QModelIndex i = ui->backupView->currentIndex();
    if (i.isValid()) {

        QList<QAction*> listActions;
        QAction *actionState10DaysAgo = new QAction("Jobs der letzten 10 Tage", this);
        listActions << actionState10DaysAgo;
        connect(actionState10DaysAgo, SIGNAL(triggered()), this, SLOT(showState10DaysAgo()) );
        actionState10DaysAgo->setData(item->getCustomerId());

        if (item->getItemType() != BackupMonitorItem::MAIN_GROUP) {

            QAction *actionDetails = new QAction("Details", this);
            actionDetails->setData(item->getJobId());
            listActions << actionDetails;
            connect(actionDetails, SIGNAL(triggered()), this, SLOT(showJobWindow()) );
        }

        QMenu::exec(listActions, ui->backupView->mapToGlobal(pos) + QPoint(0, 25) );
    }
}

// Zeigt die Jobs des aktuell markierten Kunden der letzten 10 Tage..
void BackupMonitorListJobs::showState10DaysAgo() {

    BackupMonitorListJobs *monitorWindow = new BackupMonitorListJobs(false);

    QVariant customerId = static_cast<QAction*>(sender())->data();
    QAction* action = static_cast<QAction*>(sender());
    qDebug() << "id: " << customerId;
    // Einstellungen setzen:
    monitorWindow->ui->radioViewCustomer->setChecked(true);
    monitorWindow->ui->inputCustomerViewDateFrom->setDate( QDate::currentDate().addDays(-10) );
    monitorWindow->ui->inputCustomerViewDateTo->setDate( QDate::currentDate().addDays(-1) );

    // In der Kundenliste einen leeren Eintrag mit der KundenID erstellen, damit dieser Kunde
    // aufgelistet wird:
    monitorWindow->ui->comboCustomers->addItem("-customerame-", customerId);

    monitorWindow->reloadBackupMonitor();

    //monitorWindow->onlyShowBackupView();

    // Namen des Kunden ermitteln und in die Titelleiste setzen:
    CREATE_TCP_COMMAND(getCustomer, customers, getCustomer);
    getCustomer->setCustomerId(customerId.toInt());
    getCustomer->setColumns( QStringList() << "customer_name" );
    getCustomer->startWait();
    QHash<QString, QVariant> row = getCustomer->fetchRow();

    monitorWindow->setWindowTitle( row["customer_name"].toString() + " - Jobs der letzten 10 Tage" );
    monitorWindow->ui->backupView->expandAll();

    monitorWindow->show();
}

// Öffnet das Details- Fenster zu einem Job (bei Doppelklick):
void BackupMonitorListJobs::showJobWindow() {

    BackupMonitorViewJob *j = new BackupMonitorViewJob(
                    static_cast<QAction*>(sender())->data().toInt()
                );
    j->show();
}

// Blendet alles bis auf das backupView aus...
void BackupMonitorListJobs::onlyShowBackupView() {

    ui->groupBoxFilterCustomer->hide();
    ui->groupBoxFilterDate->hide();

    ui->pushReload->hide();
    ui->radioViewCustomer->hide();
    ui->radioViewDate->hide();
    ui->labelView->hide();
    ui->label_2->hide();
    ui->label_3->hide();
    ui->label_4->hide();
    ui->label_5->hide();
    ui->label_6->hide();

    ui->radioViewDate->hide();

    delete ui->horizontalViewLayout;
}

// Läd die Anzeige des Monitors neu...
void BackupMonitorListJobs::reloadBackupMonitor() {

    if (ui->radioViewDate->isChecked()) {
        /**
          * Datumsansicht ist aktiviert:
          */
        _backupModel->setGroupBy(BackupMonitorItemModel::BY_CUSTOMER);
        _backupModel->setCustomerId(-1);
        _backupModel->setDateFrom(ui->inputDateViewDateFrom->date());
        _backupModel->setDateTo(ui->inputDateViewDateTo->date());
    }
    else {
        /**
          * Kundenansicht ist aktiviert:
          */
        qDebug() << "customerId: " << ui->comboCustomers->itemData(ui->comboCustomers->currentIndex()).toInt();
        qDebug() << ui->comboCustomers->count();
        _backupModel->setGroupBy(BackupMonitorItemModel::BY_DATE);
        _backupModel->setCustomerId( ui->comboCustomers->itemData(ui->comboCustomers->currentIndex()).toInt() );
        _backupModel->setDateFrom(ui->inputCustomerViewDateFrom->date());
        _backupModel->setDateTo(ui->inputCustomerViewDateTo->date());
    }

    _backupModel->reload();
}
