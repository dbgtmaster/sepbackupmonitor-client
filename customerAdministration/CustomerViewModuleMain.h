#ifndef CUSTOMERVIEWMODULEMAIN_H
#define CUSTOMERVIEWMODULEMAIN_H

#include "customerAdministration/CustomerViewModule.h"

namespace Ui {
class CustomerViewModuleMain;
}

class CustomerViewModuleMain : public CustomerViewModule
{
    Q_OBJECT
    
private:
    bool _isEditable;

public:
    explicit CustomerViewModuleMain(QWidget *parent = 0);
    ~CustomerViewModuleMain();

    inline QStringList getNeededCustomerColumns() {
        return QStringList() << "customer_name" << "address" << "plz" << "city"
                             << "main_email" << "id";
    }

    void setCustomerEditable(bool b);

    QHash<QString, QVariant> save();

    void init(const QHash<QString, QVariant> &row);
    
private:
    Ui::CustomerViewModuleMain *ui;
};

#endif // CUSTOMERVIEWMODULEMAIN_H
