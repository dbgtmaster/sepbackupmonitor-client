#ifndef BACKUPMONITOREXPORTTHREAD_H
#define BACKUPMONITOREXPORTTHREAD_H

#include <QThread>
#include <QListWidgetItem>
#include <QDate>

#include <QMutex>

class BackupMonitorExport;

class BackupMonitorExportThread : public QThread
{
    Q_OBJECT

    QList< QPair<int, QString > > _customers;
    QDate _dateFrom;
    QDate _dateTo;
    QString _destDir;

    bool _aborted;
    QMutex _mutexAborted;

public:
    /**
     * ACHTUNG: Bevor das uiWindow übergeben wird, müssen alle Angaben überprüft werden!
     */
    BackupMonitorExportThread(QObject *parent = 0);

    void setData(QList< QPair<int, QString> > customers, const QDate &dateFrom,
                 const QDate &dateTo, const QString &destDir);

    void run();

    void stop();

private:

    QString _htmlContent;

    inline void _appendHtml(const QString html) { _htmlContent += html + "\n"; }

    // Wird zum Initialisieren des Exporters aufgerufen:
    void _initExportData(quint32 _customerId, const QString &_customerName, const QDate &startDate, const QDate &endDate);

    // Wird aufgerufen, wenn ein neuer Backuptag exportiert wird
    void _appendNextDay(const QDate &d);

    // Wird aufgerufen, wenn zum aktuellen Backup- Tag ein Job hinzugefügt wird:
    void _appendJob(QHash<QString, QVariant> data);

    // Wenn der aktuelle Tag abgeschlossen ist.
    void _dayFinished(int countJobs);

    // Wird aufgerufen, wenn der Export fertig gestellt wird.
    void _exportDataFinished(quint32 customerId, const QString &customerName);

    // Überprüft, ob dem Thread übermittelt wurde, ob sich dieser beenden soll..
    bool _checkIfAbortReceived();

signals:
    // Übermittelt den Status der Protokoll- Tage:
    // int done: Wie viele Protokolltage wurden bereits erledigt.
    // int total: Wie viele Protokolltage sind es insgesamt..
    void state(int done, int total);
    void stateMessage(QString message);
    void errorMessage(QString message);
};

#endif // BACKUPMONITOREXPORTTHREAD_H
