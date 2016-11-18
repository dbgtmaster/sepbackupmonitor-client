#ifndef BACKUPMONITOREXPORT_H
#define BACKUPMONITOREXPORT_H

#include <QWidget>

namespace Ui {
class BackupMonitorExport;
}

class BackupMonitorExport : public QWidget
{
    Q_OBJECT
    
    friend class BackupMonitorExportThread;

public:
    explicit BackupMonitorExport(QWidget *parent = 0);
    ~BackupMonitorExport();
    
public slots:
    void create();
    void chooseFolder();

    // Wird aufgerufen, wenn der Export abgeschlossen wurde..
    void exportFinished();
private:
    Ui::BackupMonitorExport *ui;
};

#endif // BACKUPMONITOREXPORT_H
