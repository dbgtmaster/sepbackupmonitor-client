#ifndef BACKUPMONITORITEMMODEL_H
#define BACKUPMONITORITEMMODEL_H

#include <QStandardItemModel>

#include <QDate>

class BackupMonitorItem;

class BackupMonitorItemModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum BackupGroupBy {
        BY_DATE,        // Jobs nach Kunden gruppiert anzeigen
        BY_CUSTOMER     // Jobs nach Datum gruppiert anzeigen
    };

    enum BackupJobType {
        JOB = 0,
        GROUP = 1
    };

private:
    BackupGroupBy _groupBy;
    QDate _dateFrom;
    QDate _dateTo;
    quint32 _customerId;

    // Alle bereits angelegten Gruppierungen...
    // Das Äussere QHash stellt alle Kundennamen bzw. die Datums dar,
    // je nachdem, welche Ansicht gewählt wurde.
    // Das innere QHash stellt zB. zu den Kundennamen alle Gruppen dar.
    // "-1" beim inneren QString bedeutet, dass Job keiner Gruppe zugeordnet ist
    // und zeigt somit direkt am Kundennamen...
    QHash<QString, QHash<QString, BackupMonitorItem*> > _insertedGroups;

    // Löscht alle Daten des Models...
    void _clear();

    // Fügt einen neuen Job hinzu..
    // QString groupBy: Unter welcher Gruppierung soll dieser Job gespeichert werden??
    //                  zB. Kundennamen, oder Datum,...
    // QHash<QString, QVariant> beinhaltet den Datensatz des Jobs..
    void _addJob(const QString &groupBy, const QHash<QString, QVariant> &job);

public:

    explicit BackupMonitorItemModel(QObject *parent = 0);
    
    inline void setGroupBy(BackupGroupBy view);
    inline void setDateFrom(const QDate &d);
    inline void setDateTo(const QDate &d);
    inline void setCustomerId(quint32 id);

    // Lädt die Daten des Models neu.
    void reload();

signals:
    
public slots:
    
};

void BackupMonitorItemModel::setGroupBy(BackupGroupBy view) {
    _groupBy = view;
}

void BackupMonitorItemModel::setDateFrom(const QDate &d) {
    _dateFrom = d;
}

void BackupMonitorItemModel::setDateTo(const QDate &d) {
    _dateTo = d;
}

void BackupMonitorItemModel::setCustomerId(quint32 id) {
    _customerId = id;
}

#endif // BACKUPMONITORITEMMODEL_H
