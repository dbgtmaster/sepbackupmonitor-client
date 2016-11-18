#ifndef BACKUPMONITORCLIENTSTATUSITEMMODEL_H
#define BACKUPMONITORCLIENTSTATUSITEMMODEL_H

#include <QStandardItemModel>
#include <QDate>

#include "BackupMonitor/BackupMonitorClientStatusItem.h"
#include "BackupMonitor/BackupMonitorGlobals.h"

class QTreeView;

class BackupMonitorClientStatusItemModel : public QStandardItemModel
{
    Q_OBJECT

    friend class BackupMonitorClientStatus;
public:

    // Sollen die Gruppen, in welche Jobs untergeordnet sind, ausgeblendet werden??
    // Die darunterbefindlichen Jobs werden so angezeigt, als wären diese in keiner Gruppe..
    bool _hideJobGroups;

    bool _showDateHeadline;     // Ob Überschriftszeile mit dem Datum angezeigt werden soll.

private:
    // Alle bereits angelegten Kunden bzw. deren Jobs...
    // Das Äussere QHash stellt alle Kundennamen dar,
    // Das innere QHash stellt zB. zu den Kundennamen alle Gruppen dar.
    // "-1" beim inneren QString bedeutet, dass Job keiner Gruppe zugeordnet ist
    // und zeigt somit direkt am Kundennamen...
    QHash<QString, QHash<QString, BackupMonitorClientStatusItem*> > _insertedGroups;

    // Eine Liste, in welcher wir die ID jeder Kunden- Gruppe speichern + das dazugehörige StandardItem
    QHash<quint32, QList<QStandardItem*> > _customerGroupsToItems;

    // Fügt einen neuen Job hinzu..
    // QHash<QString, QVariant> beinhaltet den Datensatz des Jobs..
    void _addJob(const QString &groupBy, QHash<QString, QVariant> &job, bool showDateHeadline);
    QHash<QString, QDate> _headlineJobLastDate;  // Das Datum des zuletzt eingefügten Jobs eines Kunden

    // View, in welchem dieses Model eingefügt wurde.
    QTreeView *_view;

public:
    BackupMonitorClientStatusItemModel(QTreeView* view, QObject *parent = 0);

    // Läd das Model neu und zeigt den Status der Clients an...
    // Qdate d: Datum, welches angezeigt werden soll.
    // QList<QIndexModel>*: Referenziert eine Liste mit die Indexe, welche expanded werden müssen.
    void reloadClientStatus(const QDate &d);

    // Läd das Model neu...
    // int customerId: Gibt die Kundenid an, welche angezeigt werden soll.
    //                 -1 bedeutet, dass alle Kunden angezeigt werden.
    // QDate start:    Datum, ab welchem die Einträge angezeigt werden sollen.
    // QDate end:      Datum, bis zu welchem die Kundeneinträge angezeigt werden soll
    void reloadSearchFilter(int customerId, QString jobFilter, QDate start, QDate end);


    void setHorizontalHeaderLabelsCustomers();
    void setHorizontalHeaderLabelsJobs();

    void clear();

    inline void setHideJobsGroups(bool b);

    // Schiebt einen Kunden in eine andere Gruppe.
    void moveCustomerToGroup(quint32 customerId, quint32 groupId);

signals:
    /**
     * @brief Übermittelt den Status über die aktuelle Aktion
     * @param state:        Text, was gerade passiert
     * @param now           Wie viele Kunden schon geladen wurden
     * @param total         Wie viele Kunden insgesamt geladen werden..
     */
    void stateLoading(QString state, int now, int total);
};

void BackupMonitorClientStatusItemModel::setHideJobsGroups(bool b) {
    _hideJobGroups = true;
}

#endif // BACKUPMONITORCLIENTSTATUSITEMMODEL_H
