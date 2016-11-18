#include "CustomerViewModuleMain.h"
#include "ui_CustomerViewModuleMain.h"

#include <QMessageBox>

CustomerViewModuleMain::CustomerViewModuleMain(QWidget *parent) :
    CustomerViewModule(parent),
    ui(new Ui::CustomerViewModuleMain), _isEditable(false)
{
    ui->setupUi(this);

    setCustomerEditable(_isEditable);

    connect(ui->pushDelete, SIGNAL(clicked()), parent, SLOT(removeCustomer()));
}

CustomerViewModuleMain::~CustomerViewModuleMain()
{
    delete ui;
}

void CustomerViewModuleMain::init(const QHash<QString, QVariant> &row) {

    ui->inputCustomerName->setText( row["customer_name"].toString() );
    ui->inputAddress->setText( row["address"].toString() );
    ui->inputPlz->setText( row["plz"].toString() );
    ui->inputCity->setText( row["city"].toString() );
    ui->inputContactEMail->setText( row["main_email"].toString() );

    if (row["plz"].toString().isEmpty()) {
        ui->inputPlz->setText( "0" );
    }
}

void CustomerViewModuleMain::setCustomerEditable(bool b) {

    _isEditable = b;

    ui->inputAddress->setReadOnly(!b);
    ui->inputCity->setReadOnly(!b);
    ui->inputContactEMail->setReadOnly(!b);
    ui->inputCustomerName->setReadOnly(!b);
    ui->inputPlz->setReadOnly(!b);

    ui->pushDelete->setVisible(b);
}

QHash<QString, QVariant> CustomerViewModuleMain::save() {

    QHash<QString, QVariant> row;

    // Nur wenn aktiviert wurde, dass Kundendaten bearbeitet werden können (wenn diese nicht von extern importiert werden)...
    if (_isEditable) {
        row["customer_name"] = ui->inputCustomerName->text();
        row["address"] = ui->inputAddress->text();
        row["plz"] = ui->inputPlz->text();
        row["city"] = ui->inputCity->text();
        row["main_email"] = ui->inputContactEMail->text();
    }

    return row;
}
