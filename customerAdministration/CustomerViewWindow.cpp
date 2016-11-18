#include "CustomerViewWindow.h"
#include "ui_CustomerViewWindow.h"

#include <QStringList>
#include <QMessageBox>

#include "TcpCommands/customers/getCustomer.h"
#include "TcpCommands/customers/modCustomer.h"
#include "TcpCommands/customers/customerEditable.h"
#include "TcpCommands/customers/removeCustomer.h"

#include "customerAdministration/CustomerViewModuleMain.h"
#include "customerAdministration/CustomerViewModuleBackupMonitor.h"

/**
 * @brief CustomerViewWindow::CustomerViewWindow
 * @param customerId:       0 -> Dialog für neuen Kunden
 * @param parent
 */
CustomerViewWindow::CustomerViewWindow(int customerId, QWidget *parent) :
    QWidget(parent), _customerId(customerId),
    ui(new Ui::CustomerViewWindow)
{
    ui->setupUi(this);


    QHash<QString, QVariant> row;

    CustomerViewModuleMain *customerMainModule = new CustomerViewModuleMain(this);
    ui->tabWidget->addTab(customerMainModule, "Allgemein");

    if (customerId != 0) {
        ui->tabWidget->addTab(new CustomerViewModuleBackupMonitor, "Backup Monitor");

        // Nun ermitteln wir alle Spalten, welche wir von dem Kunden benötigen:
        QStringList columns;
        columns << "customer_name";
        int count = ui->tabWidget->count();
        for (int i = 0; i < count; i++) {
            columns.append( static_cast<CustomerViewModule*>(ui->tabWidget->widget(i))
                         ->getNeededCustomerColumns() );
        }

        CREATE_TCP_COMMAND(getCustomer, customers, getCustomer);
        getCustomer->setCustomerId(customerId);
        getCustomer->setColumns( columns );
        getCustomer->startWait();

        row = getCustomer->fetchRow();

        setWindowTitle( "Kunde: " + row["customer_name"].toString() );

    }
    else {
        setWindowTitle("Neuer Kunde");
    }

    // Können Kundendaten bearbeitet werden?
    CREATE_TCP_COMMAND(editable, customers, customerEditable);
    editable->startWait();
    _customerDataEditable = editable->isEditable();

    customerMainModule->setCustomerEditable(_customerDataEditable);

    // Und nun initialisieren wir alle Module:
    int c = ui->tabWidget->count();
    for (int i = 0; i < c; i++) {
        static_cast<CustomerViewModule*>(ui->tabWidget->widget(i))->init(row);
    }

    connect(ui->pushOk, SIGNAL(clicked()), this, SLOT(saveSettings()));
    connect(ui->pushCancel, SIGNAL(clicked()), this, SLOT(close()));
}

CustomerViewWindow::~CustomerViewWindow()
{
    delete ui;
}

void CustomerViewWindow::saveSettings() {

    /**
     * Wenn ein neuer Kudne erstellt wird, so wird als _customerId 0 übergeben...
     */

    QHash<QString, QVariant> row;

    // Zu änderte Einstellungen aller Module holen:
    int c = ui->tabWidget->count();
    for (int i = 0; i < c; i++) {
        row.unite( static_cast<CustomerViewModule*>(ui->tabWidget->widget(i))->save() );
    }

    CREATE_TCP_COMMAND(mod, customers, modCustomer);
    mod->setCustomerId(_customerId);        // Beim erstellen wird ID 0 übergeben...
    mod->setModColumns(row);
    mod->startWait();

    if (!mod->successfulModified()) {

        QMessageBox::critical(this, "Fehler",
                              QString("Beim Speichern der Änderungen ist ein Fehler aufgetreten:<br>%1").arg(mod->getErrorMessage()));
        return;
    }

    if (_customerId == 0) {
        // Kunde wurde angelegt, wir öffnen den angelegten Kunden:

        QMessageBox::information(this, "Kunde angelegt", "Kunde wurde erfolgreich angelegt. Erstellter Kunde wird in einem neuen Fenster geöffnet.");

        CustomerViewWindow *window = new CustomerViewWindow(mod->getCustomerId());
        window->show();
        close();
    }
    else {
        // Kunde wurde bearbeitet.
        close();
    }
}

void CustomerViewWindow::removeCustomer() {

    int state = QMessageBox::question(this, trUtf8("Kunde löschen"), "Soll dieser Kunde wirklich gelöscht werden?", QMessageBox::Yes, QMessageBox::No);
    if (state == QMessageBox::Yes) {

        // Kunde löschen:
        CREATE_TCP_COMMAND(remove, customers, removeCustomer);
        remove->setCustomerId(_customerId);
        remove->startWait();

        QMessageBox::information(this, trUtf8("Kunde gelöscht"), "Kunde wurde erfolgreich gelöscht");
        close();
    }
}
