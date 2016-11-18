 #include "CustomerAdministration.h"
#include "ui_CustomerAdministration.h"

#include <QStandardItemModel>
#include <QDebug>
#include <QMessageBox>

#include <TcpCommands/customers/search.h>
#include <TcpCommands/customers/customerEditable.h>

#include "customerAdministration/CustomerViewWindow.h"

CustomerAdministration::CustomerAdministration(QWidget *parent) :
    MdiAbstractWindow(parent),
    ui(new Ui::CustomerAdministration)
{
    ui->setupUi(this);

    _customerTableModel = new QStandardItemModel;

    connect(ui->inputSearch, SIGNAL(returnPressed()), ui->pushSearch, SIGNAL(clicked()));
    connect(ui->pushSearch, SIGNAL(clicked()), this, SLOT(search()));
    connect(ui->pushNewCustomer, SIGNAL(clicked()), this, SLOT(create()));

    _customerTableModel->setHorizontalHeaderLabels( QStringList() << "Kundenname" <<
                                                    "Adresse" << "PLZ" << "Ort" );

    ui->treeViewCustomers->setModel( _customerTableModel );
    ui->treeViewCustomers->setColumnWidth(0, 400);
    ui->treeViewCustomers->setColumnWidth(1, 260);
    ui->treeViewCustomers->setColumnWidth(2, 45);
    ui->treeViewCustomers->setColumnWidth(3, 300);

    ui->treeViewCustomers->setRootIsDecorated(false);
    ui->treeViewCustomers->setSortingEnabled(true);
    ui->treeViewCustomers->sortByColumn(0, Qt::AscendingOrder);

    connect(ui->treeViewCustomers, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(customerClicked(QModelIndex)));

    CREATE_TCP_COMMAND(editable, customers, customerEditable);
    editable->startWait();
    _customerDataEditable = editable->isEditable();
    if (!_customerDataEditable) {
        ui->pushNewCustomer->setVisible(false);
    }
}

CustomerAdministration::~CustomerAdministration()
{
    delete ui;
}

void CustomerAdministration::customerClicked(const QModelIndex &index) {

    int id = _customerTableModel->itemFromIndex(index)->data(Qt::UserRole).toInt();

    CustomerViewWindow* view = new CustomerViewWindow(id);
    view->show();
}

/**
 * @brief CustomerAdministration::create
 *
 * Erstellt einen neuen Kunden
 */
void CustomerAdministration::create() {

    CustomerViewWindow* view = new CustomerViewWindow(0);
    view->show();
}

void CustomerAdministration::search() {

    // Aktuellen Inhalt der Tabelle leeren:
    int count = _customerTableModel->rowCount();
    for (int i = 0; i < count; i++) {
        _customerTableModel->takeRow(0).clear();
    }

    CREATE_TCP_COMMAND(search, customers, search);
    search->setSearchFilter(ui->inputSearch->text());

    // Welche Spalten sollen mit dem Filter durchsucht werden?
    QStringList filterColumns;
    if (ui->checkFilterCustomerName->isChecked())   filterColumns << "customer_name";
    if (ui->checkFilterAddress->isChecked())        filterColumns << "address";
    if (ui->checkFilterPLZ->isChecked())            filterColumns << "plz";
    if (ui->checkFilterCity->isChecked())           filterColumns << "city";

    if (filterColumns.count() == 0) {
        QMessageBox::critical(this, "Fehler", "Es wurden keine Suchfilter ausgewählt!");
        return;
    }

    search->setFilterColumns(filterColumns);

    search->setReturnColumns( QStringList() << "id" << "customer_name" << "address" << "plz" << "city" );

    search->startWait();

    QHash<QString, QVariant> row;
    int rowCount = 0;
    while(search->hasNextCustomer()) {

        row = search->fetchNextCustomer();
        int id = row["id"].toInt();

        {
            QStandardItem* item = new QStandardItem( row["customer_name"].toString() );
            item->setData(id, Qt::UserRole);
            item->setFlags( item->flags() & ~Qt::ItemIsEditable );
            _customerTableModel->setItem(rowCount, 0, item );
        }

        {
            QStandardItem* item = new QStandardItem( row["address"].toString() );
            item->setData(id, Qt::UserRole);
            item->setFlags( item->flags() & ~Qt::ItemIsEditable );
            _customerTableModel->setItem(rowCount, 1, item );
        }

        {
            QStandardItem* item = new QStandardItem( row["plz"].toString() );
            item->setData(id, Qt::UserRole);
            item->setFlags( item->flags() & ~Qt::ItemIsEditable );
            _customerTableModel->setItem(rowCount, 2, item );
        }

        {
            QStandardItem* item = new QStandardItem( row["city"].toString() );
            item->setData(id, Qt::UserRole);
            item->setFlags( item->flags() & ~Qt::ItemIsEditable );
            _customerTableModel->setItem(rowCount, 3, item );
        }

        rowCount++;
    }

    // Damit nach dem einfügen der Einträge neu sortiert wird:
    ui->treeViewCustomers->setSortingEnabled(true);
}
