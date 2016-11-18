#ifndef CUSTOMERVIEWMODULEBACKUPMONITOR_H
#define CUSTOMERVIEWMODULEBACKUPMONITOR_H

#include "customerAdministration/CustomerViewModule.h"

namespace Ui {
class CustomerViewModuleBackupMonitor;
}

class CustomerViewModuleBackupMonitor : public CustomerViewModule
{
    Q_OBJECT
    
private:
    quint32 _customerId;
    QString _customerInterfaceAddress;

public:
    explicit CustomerViewModuleBackupMonitor(QWidget *parent = 0);
    ~CustomerViewModuleBackupMonitor();
    
    inline QStringList getNeededCustomerColumns() {
        return QStringList() << "backup_enabled" << "backup_interface_key" << "id"
                             << "backup_prospective_jobs_mon" << "backup_prospective_jobs_tue"
                             << "backup_prospective_jobs_wed" << "backup_prospective_jobs_thu"
                             << "backup_prospective_jobs_fri" << "backup_prospective_jobs_sat"
                             << "backup_prospective_jobs_sun";
    }

    void init(const QHash<QString, QVariant> &row);

    QHash<QString, QVariant> save();

public slots:
    void disableMonitor();
    void enableMonitor();

    void generateNewInterfaceKey();
    void clearProxyInputs();

    void reloadClientInstallCommand();

    void openBrowserMsiDownload();

private:
    Ui::CustomerViewModuleBackupMonitor *ui;
};

#endif // CUSTOMERVIEWMODULEBACKUPMONITOR_H
