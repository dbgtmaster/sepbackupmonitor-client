#ifndef MDIABSTRACTWINDOWFACTORY_H
#define MDIABSTRACTWINDOWFACTORY_H

#include "MainWindow/MdiAbstractWindow.h"

class MdiAbstractWindowFactory
{
public:
    MdiAbstractWindowFactory();

    // Muss von geerbten MdiWindow überschrieben werden, welches das MdiWindow erstellt..
    virtual MdiAbstractWindow* createWindow() = 0;
};

#endif // MDIABSTRACTWINDOWFACTORY_H
