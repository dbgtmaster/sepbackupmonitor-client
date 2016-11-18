#ifndef CUSTOMERADMINISTRATIONFACTORY_H
#define CUSTOMERADMINISTRATIONFACTORY_H

#include "MainWindow/MdiAbstractWindowFactory.h"

#include "customerAdministration/CustomerAdministration.h"

class CustomerAdministrationFactory : public MdiAbstractWindowFactory
{
public:
    CustomerAdministrationFactory();

    inline MdiAbstractWindow* createWindow();
};

MdiAbstractWindow* CustomerAdministrationFactory::createWindow() {
    return new CustomerAdministration;
}

#endif // CUSTOMERADMINISTRATIONFACTORY_H
