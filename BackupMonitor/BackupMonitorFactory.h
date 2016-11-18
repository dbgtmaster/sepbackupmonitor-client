#ifndef BACKUPMONITORFACTORY_H
#define BACKUPMONITORFACTORY_H

#include "MainWindow/MdiAbstractWindowFactory.h"

#include "BackupMonitor.h"

class BackupMonitorFactory : public MdiAbstractWindowFactory
{
public:
    BackupMonitorFactory();

    inline MdiAbstractWindow* createWindow();
};

MdiAbstractWindow* BackupMonitorFactory::createWindow() {
    return new BackupMonitor;
}

#endif // BACKUPMONITORFACTORY_H
