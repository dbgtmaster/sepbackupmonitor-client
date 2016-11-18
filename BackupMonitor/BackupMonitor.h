#ifndef BACKUPMONITOR_H
#define BACKUPMONITOR_H

#include "MainWindow/MdiAbstractWindow.h"

namespace Ui {
class BackupMonitor;
}

class BackupMonitor : public MdiAbstractWindow
{
    Q_OBJECT
    
public:
    explicit BackupMonitor(QWidget *parent = 0);
    ~BackupMonitor();
    
private:
    Ui::BackupMonitor *ui;
};

#endif // BACKUPMONITOR_H
