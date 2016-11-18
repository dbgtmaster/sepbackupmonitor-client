#ifndef BACKUPMONITORMANAGEGROUPSNEW_H
#define BACKUPMONITORMANAGEGROUPSNEW_H

#include <QDialog>

namespace Ui {
class BackupMonitorManageGroupsNew;
}

class BackupMonitorManageGroupsNew : public QDialog
{
    Q_OBJECT
    
    // Ob eine Gruppe angelegt wurde...?
    bool _created;

public:
    explicit BackupMonitorManageGroupsNew(QWidget *parent = 0);
    ~BackupMonitorManageGroupsNew();

    inline bool isCreated() { return _created; }

public slots:
    void create();
    
private:
    Ui::BackupMonitorManageGroupsNew *ui;
};

#endif // BACKUPMONITORMANAGEGROUPSNEW_H
