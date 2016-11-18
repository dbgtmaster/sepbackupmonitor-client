#ifndef BACKUPMONITOREXPORTSTATUSDIALOG_H
#define BACKUPMONITOREXPORTSTATUSDIALOG_H

#include <QDialog>

class BackupMonitorExportThread;

namespace Ui {
class BackupMonitorExportStatusDialog;
}

class BackupMonitorExportStatusDialog : public QDialog
{
    Q_OBJECT
    
    BackupMonitorExportThread *_thread;

    // Ob ein oder mehrere Fehler aufgetreten sind...
    bool _errors;
    bool _finished;     // ob der Export beendet wurde.

public:
    explicit BackupMonitorExportStatusDialog(QWidget *parent = 0);
    ~BackupMonitorExportStatusDialog();
    
    virtual int exec();

    void closeEvent(QCloseEvent *);

public slots:
    void status(int protocolDaysFinished, int protocolDaysTotal);
    void exportFinished();
    void statusMessage(const QString &message);
    void errorMessage(const QString &message);
    void cancel();

    // Liefert den Thread zurück, damit BackupMonitorExport den Thread die zu bearbeitenden Daten setzen kann (setData(..))
    // Siehe BackupMonitorExportThread::setData(...)
    BackupMonitorExportThread* thread() { return _thread; }

signals:

private:
    Ui::BackupMonitorExportStatusDialog *ui;
};

#endif // BACKUPMONITOREXPORTSTATUSDIALOG_H
