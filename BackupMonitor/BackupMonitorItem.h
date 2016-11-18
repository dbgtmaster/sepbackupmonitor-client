#ifndef BACKUPMONITORITEM_H
#define BACKUPMONITORITEM_H

#include <QStandardItem>

class BackupMonitorDelegate;

class BackupMonitorItem : public QStandardItem
{
    friend class BackupMonitorDelegate;
public:
    enum TYPE { JOB,            // Es handelt sich hierbei um einen Job
                JOB_GROUP,      // Es handelt sich hierbei um eine Job- Gruppierung
                MAIN_GROUP      // Es handelt sich hierbei um eine Hauptgruppierung
                                // zB. um den Kundennamen, oder bei Datumssortierung um das Datum.
              };

private:
    TYPE _itemType;     // Gruppe oder Job

    quint32 _customerId;
    quint32 _jobId;

    // Im Falle einer Gruppe:
    quint32 _countSuccessful;
    quint32 _countWarning;
    quint32 _countError;
    quint32 _countRunning;

public:
    explicit BackupMonitorItem(const QString &text);
    
    // Setzt die ID des Kunden
    inline void setCustomerId(quint32 id);
    inline quint32 getCustomerId();

    // Setzt die ID des Jobs
    inline void setJobId(quint32 id);
    inline quint32 getJobId();

    // Handelt es sich hierbei um einen Job, oder einer Gruppe??
    inline void setItemType(TYPE t);
    inline TYPE getItemType();

    // Folgende Methoden haben nur eine Funktion, wenn es sich bei diesem
    // Job um eine Gruppe handelt...
    // Erhöht bei der Anzeige der Gruppe die Anzahl der Erfolgreichen, Warnungen,...
    inline void incrementSuccessful();
    inline void incrementWarning();
    inline void incrementError();
    inline void incrementRunning();

signals:
    
public slots:
    
};

void BackupMonitorItem::setItemType(TYPE t) {
    _itemType = t;
}

BackupMonitorItem::TYPE BackupMonitorItem::getItemType() {
    return _itemType;
}

void BackupMonitorItem::setCustomerId(quint32 id) {
    _customerId = id;
}

quint32 BackupMonitorItem::getCustomerId() {
    return _customerId;
}

void BackupMonitorItem::setJobId(quint32 id) {
    _jobId = id;
}

quint32 BackupMonitorItem::getJobId() {
    return _jobId;
}

void BackupMonitorItem::incrementSuccessful() {
    _countSuccessful++;
}

void BackupMonitorItem::incrementWarning() {
    _countWarning++;
}

void BackupMonitorItem::incrementError() {
    _countError++;
}

void BackupMonitorItem::incrementRunning() {
    _countRunning++;
}

#endif // BACKUPMONITORITEM_H
