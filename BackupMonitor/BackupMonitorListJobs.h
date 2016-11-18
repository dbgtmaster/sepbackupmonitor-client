#ifndef BACKUPMONITORLISTJOBS_H
#define BACKUPMONITORLISTJOBS_H

#include <QWidget>

#include "BackupMonitor/BackupMonitorItemModel.h"

namespace Ui {
class BackupMonitorListJobs;
}

class BackupMonitorListJobs : public QWidget
{
    Q_OBJECT
    
private:

    BackupMonitorItemModel *_backupModel;

    // Ladet die Liste der Backupkunden (neu).
    void _reloadCustomerList();

public:

    // bool loadCustomerList: Gibt an, ob die Methode _reloadCustomerList() automatisch
    //      aufgerufen werden soll. Wird zB. nicht benötigt, wenn die Kundenliste ausgeblendet
    //      wird...
    explicit BackupMonitorListJobs(bool loadCustomerList = true, QWidget *parent = 0);
    ~BackupMonitorListJobs();

    // Blendet alles aus, so dass nur mehr das Backup- View angezeigt wird...
    void onlyShowBackupView();
    
public slots:
    void reloadBackupMonitor();

    void jobClicked(QModelIndex index);
    void contextMenuRequested(QPoint);

    // Zeigt die Protokolle des ausgewählten Kunden im backupView der letzten 10 Tage...
    void showState10DaysAgo();

    // Zeigt einen Job:
    void showJobWindow();

private:
    Ui::BackupMonitorListJobs *ui;
};

#endif // BACKUPMONITORLISTJOBS_H
