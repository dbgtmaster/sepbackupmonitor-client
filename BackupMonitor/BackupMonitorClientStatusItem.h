#ifndef BACKUPMONITORCLIENTSTATUSITEM_H
#define BACKUPMONITORCLIENTSTATUSITEM_H

#include <QStandardItem>
#include <QDebug>

class BackupMonitorClientStatusItem : public QStandardItem
{
public:
    enum ITEM {
        ITEM_UNKNOWN,       // Nicht definiertes Item- Type
        ITEM_BACKUP_DAYS,
        ITEM_LAST_BACKUP_DAY,
        ITEM_RETROSPECTIVE_BACKUP,
        ITEM_LAST_LOG_DAYS
    };

    enum TYPE { CUSTOMER,
                CUSTOMER_GROUP, // Gruppe, in welche Kunden zugeordnet werden können.
                JOB,            // Es handelt sich hierbei um einen Job
                JOB_GROUP,      // Es handelt sich hierbei um eine Job- Gruppierung
                MAIN_GROUP,      // Es handelt sich hierbei um eine Hauptgruppierung
                                // zB. um den Kundennamen, oder bei Datumssortierung um das Datum.
                HEADLINE        // Stellt eine Überschrift dar...
    };

private:



    // Im Falle einer Gruppe:
    ITEM _item;
    TYPE _itemType;     // Gruppe oder Job

    quint32 _customerId;
    quint32 _jobId;
    quint32 _customerGroupId;

    QVariant _itemData;
    quint32 _countSuccessful;
    quint32 _countWarning;
    quint32 _countError;
    quint32 _countRunning;

public:
    explicit BackupMonitorClientStatusItem(const QString &text, TYPE type);
    
    void setItem(ITEM item);
    ITEM getItem();

    // Setzt die ID des Kunden
    inline void setCustomerId(quint32 id);
    inline quint32 getCustomerId();

    // Setzt die ID des Jobs
    inline void setJobId(quint32 id);
    inline quint32 getJobId();

    // Handelt es sich hierbei um einen Job, oder einer Gruppe??
    inline void setItemType(TYPE t);
    inline TYPE getItemType();

    inline void setCustomerGroupId(quint32 id) { _customerGroupId = id; }
    inline quint32 getCustomerGroupId() { return _customerGroupId; }

    void setItemData(const QVariant &data);
    QVariant getItemData();

    // Folgende Methoden haben nur eine Funktion, wenn es sich bei diesem
    // Job um eine Gruppe handelt...
    // Erhöht bei der Anzeige der Gruppe die Anzahl der Erfolgreichen, Warnungen,...
    inline void incrementSuccessful();
    inline void incrementWarning();
    inline void incrementError();
    inline void incrementRunning();

    void mouseMoveEvent();

signals:
    
public slots:
    
};

void BackupMonitorClientStatusItem::setItemType(TYPE t) {
    _itemType = t;

    if (t == HEADLINE) {
        setBackground( QBrush( QColor(211, 211, 211) ) );
    }

    if (t == CUSTOMER_GROUP) {
        setBackground( QBrush( QColor(240, 240, 240) ) );
    }
}

BackupMonitorClientStatusItem::TYPE BackupMonitorClientStatusItem::getItemType() {
    return _itemType;
}

void BackupMonitorClientStatusItem::setCustomerId(quint32 id) {
    _customerId = id;
}

quint32 BackupMonitorClientStatusItem::getCustomerId() {
    return _customerId;
}

void BackupMonitorClientStatusItem::setJobId(quint32 id) {
    _jobId = id;
}

quint32 BackupMonitorClientStatusItem::getJobId() {
    return _jobId;
}

void BackupMonitorClientStatusItem::incrementSuccessful() {
    _countSuccessful++;
}

void BackupMonitorClientStatusItem::incrementWarning() {
    _countWarning++;
}

void BackupMonitorClientStatusItem::incrementError() {
    _countError++;
}

void BackupMonitorClientStatusItem::incrementRunning() {
    _countRunning++;
}

#endif // BACKUPMONITORCLIENTSTATUSITEM_H
