#include "BackupMonitorItemModel.h"

#include <QDate>

#include "TcpCommands/backupMonitor/getJobs.h"

#include "BackupMonitor/BackupMonitorItem.h"

#include <QModelIndex>

BackupMonitorItemModel::BackupMonitorItemModel(QObject *parent) :
    QStandardItemModel(parent), _customerId(-1)
{
    setHorizontalHeaderLabels( QStringList() << "Auftragsname" << "Datum" << "Gestartet"
                               << "Beendet" << "Nachricht" );
}

void BackupMonitorItemModel::reload() {

    /**
      * Daten des Model neu laden....
      */

    _clear();

    CREATE_TCP_COMMAND(getJobs, backupMonitor, getJobs);
    getJobs->setFromDate(_dateFrom);
    getJobs->setToDate(_dateTo);
    getJobs->setCustomerId(_customerId);
    getJobs->startWait();

    // Alle Jobs anlegen:
    QList< QHash<QString, QVariant> > jobs = getJobs->getReturnedJobs();
    QHash<QString, QVariant> job;
    foreach(job, jobs) {
        if (_groupBy == BY_CUSTOMER) {
            _addJob(job["customer_name"].toString(), job);
        }
        else {
            _addJob(job["date"].toDate().toString(), job);
        }
    }
}

void BackupMonitorItemModel::_clear() {

    if (hasChildren()) {
        _insertedGroups.clear();
        invisibleRootItem()->removeRows(0, rowCount());
    }
}

void BackupMonitorItemModel::_addJob(const QString &groupBy, const QHash<QString, QVariant> &job) {

    if (_insertedGroups[groupBy]["-1"] == 0) {

        // Gruppierung (Kundenname bzw. Datum) wurde noch nicht angelegt...
        BackupMonitorItem *item =  new BackupMonitorItem(groupBy);
        _insertedGroups[groupBy]["-1"] = item;

        item->setEditable(false);

        item->setCustomerId(job["customer_id"].toInt());
        item->setItemType( BackupMonitorItem::MAIN_GROUP );

        // Item, auf welchem der Status des Kunden gezeichnet wird (wie viele erfolgreich,...)
        BackupMonitorItem *statusItem =  new BackupMonitorItem("");
        statusItem->setEditable(false);
        statusItem->setItemType( BackupMonitorItem::MAIN_GROUP );
        _insertedGroups[groupBy]["-2"] = statusItem;

        appendRow( QList<QStandardItem*>() << item << statusItem );
    }

    // Existiert vom Job die Übergeordnete Gruppe schon??
    // Wenn nicht, so legen wir einen "fake" davon an, welcher
    // dann von der Gruppe, sobald diese angelegt wird, überschrieben wird...
    if (job["group_softwareid"].toString() != "-1" &&
            _insertedGroups[groupBy][job["group_softwareid"].toString()] == 0) {

        BackupMonitorItem *item =  new BackupMonitorItem("PRE-CREATED GROUP " + job["group_softwareid"].toString());
        _insertedGroups[groupBy][ job["group_softwareid"].toString() ] = item;

        _insertedGroups[groupBy]["-1"]->appendRow( item );
    }

    /**
      * Handelt es sich hierbei um einen Job oder einer Gruppe??
      */
    if (job["type"].toInt() == JOB) {

        /**
          * Es ist ein JOB:
          */

        QList<QStandardItem*> items;

        items << new BackupMonitorItem(job["taskname"].toString())
              << new BackupMonitorItem(job["date"].toDate().toString())
              << new BackupMonitorItem(job["start_time"].toTime().toString())
              << new BackupMonitorItem(job["end_time"].toTime().toString())
              << new BackupMonitorItem(job["msg"].toString());

        // Status- Farbe setzen:
        if (job["state"].toInt() == 0 || job["state"].toInt() == 50) {
            // state 50: Tagesprotokoll
            // Erfolgreich:
            items[0]->setIcon( QIcon(":/green.png") );
            if (job["taskname"].toString() != "sm_newday"
                    && job["taskname"].toString() != "Tagesprotokoll") {
                _insertedGroups[groupBy]["-2"]->incrementSuccessful();
            }
        }
        else if (job["state"].toInt() == 1) {
            // Warnungen:
            items[0]->setIcon( QIcon(":/yellow.png") );
            if (job["taskname"].toString() != "sm_newday"
                    && job["taskname"].toString() != "Tagesprotokoll") {
                _insertedGroups[groupBy]["-2"]->incrementWarning();
            }
        }
        else if (job["state"].toInt() == 80 || job["state"].toInt() == 6) {
            // Laufende Jobs oder wartende, die darauf warten, gestartet zu werden:
            items[0]->setIcon( QIcon(":/blue.png") );
            if (job["taskname"].toString() != "sm_newday"
                    && job["taskname"].toString() != "Tagesprotokoll") {
                _insertedGroups[groupBy]["-2"]->incrementRunning();
            }
        }
        else {
            // Alles andere wird als Fehler behandelt:
            items[0]->setIcon( QIcon(":/red.png") );
            if (job["taskname"].toString() != "sm_newday"
                    && job["taskname"].toString() != "Tagesprotokoll") {
                _insertedGroups[groupBy]["-2"]->incrementError();
            }
        }

        // Auf alle Items Standard- Werte setzen:
        foreach(QStandardItem *i, items) {
            i->setEditable(false);

            static_cast<BackupMonitorItem*>(i)->setJobId(job["id"].toInt());
            static_cast<BackupMonitorItem*>(i)->setCustomerId(job["customer_id"].toInt());
            static_cast<BackupMonitorItem*>(i)->setItemType(BackupMonitorItem::JOB);
        }

        // Job anlegen:
        // Tagesprotokolle werden immer ganz am Beginn eingefügt:
        if (job["software_id"].toString().startsWith("DP")) {
            // Tagesprotokoll:
            _insertedGroups[groupBy][job["group_softwareid"].toString()]->insertRow(0, items );
        }
        else {
            _insertedGroups[groupBy][job["group_softwareid"].toString()]->appendRow(items );
        }

    }
    else {

        /**
          * Es ist eine Gruppe:
          */

        QList<QStandardItem*> items;

        // Falls schon eine "PRE CREATED" Gruppe angelegt wurde (weil bereits ein Job im Model angelegt
        // wurde, welcher sich in dieser Gruppe befindet), so bearbeiten wir diesen, ansonsten wird diese
        // Gruppe neu angelegt.
        BackupMonitorItem* firstGroupItem = _insertedGroups[groupBy][ job["software_id"].toString( ) ];
        if (firstGroupItem == 0) {
            items << new BackupMonitorItem(job["taskname"].toString());
        }
        else {

            firstGroupItem->setText(job["taskname"].toString());
            items << firstGroupItem;
        }


        items << new BackupMonitorItem(job["date"].toDate().toString())
              << new BackupMonitorItem(job["start_time"].toTime().toString())
              << new BackupMonitorItem(job["end_time"].toTime().toString())
              << new BackupMonitorItem(job["msg"].toString());

        // Status- Farbe setzen:
        if (job["state"].toInt() == 0) {
            // Erfolgreich:
            items[0]->setIcon( QIcon(":/green.png") );
        }
        else if (job["state"].toInt() == 1) {
            // Warnungen:
            items[0]->setIcon( QIcon(":/yellow.png") );
        }
        else if (job["state"].toInt() == 80 || job["state"].toInt() == 6) {
            // Laufende Jobs oder wartende, die darauf warten, gestartet zu werden:
            items[0]->setIcon( QIcon(":/blue.png") );
        }
        else {
            // Alles andere wird als Fehler behandelt:
            items[0]->setIcon( QIcon(":/red.png") );
        }

        // Auf alle Items Standard- Werte setzen:
        foreach(QStandardItem *i, items) {
            i->setEditable(false);

            static_cast<BackupMonitorItem*>(i)->setJobId(job["id"].toInt());
            static_cast<BackupMonitorItem*>(i)->setCustomerId(job["customer_id"].toInt());
            static_cast<BackupMonitorItem*>(i)->setItemType(BackupMonitorItem::JOB_GROUP);
        }

        // Falls schon eine "PRE CREATED" Gruppe angelegt wurde (weil bereits ein Job im Model angelegt
        // wurde, welcher sich in dieser Gruppe befindet), so bearbeiten wir diesen, ansonsten wird diese
        // Gruppe neu angelegt.
        if (_insertedGroups[groupBy][job["software_id"].toString()] == 0) {

            // Im Model anlegen:
            _insertedGroups[groupBy]["-1"]->appendRow( QList<QStandardItem*>() << items );

            // Im eingefügten Gruppen- Cache anlegen:
            _insertedGroups[groupBy][ job["software_id"].toString() ] = static_cast<BackupMonitorItem*>(items[0]);
        }
        else {

            // Gruppe wurde bereits mit "PRE- CREATED" angelegt.
            // Spalte 1 bis XX werden eingefügt...
            for (int i = 1; i < items.count(); i++) {
                firstGroupItem->parent()->setChild( firstGroupItem->row(), i, items[i] );
            }
        }
    }

}
