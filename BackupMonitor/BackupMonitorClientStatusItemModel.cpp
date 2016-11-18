#include "BackupMonitorClientStatusItemModel.h"

#include "BackupMonitor/BackupMonitorClientStatusItem.h"

#include "TcpCommands/customers/search.h"
#include "TcpCommands/backupMonitor/getJobs.h"
#include "TcpCommands/backupMonitor/getBackupStateOfDay.h"
#include "TcpCommands/backupMonitor/getCustomerGroups.h"

#include <QDateTime>
#include <QLabel>
#include <QTreeView>

#include "Utils/UtilDateTime.h"

BackupMonitorClientStatusItemModel::BackupMonitorClientStatusItemModel(QTreeView *view, QObject *parent) :
    QStandardItemModel(parent), _hideJobGroups(false), _showDateHeadline(true), _view(view)
{

}

void BackupMonitorClientStatusItemModel::setHorizontalHeaderLabelsCustomers() {

    setHorizontalHeaderLabels( QStringList() << "Kundenname" << "Version" << "letzter Abgleich" << "Last log"
                               << "Status letztes Backup" << trUtf8("Rückblickend") << "Backup- Tage" << "Status" );
}

void BackupMonitorClientStatusItemModel::setHorizontalHeaderLabelsJobs() {

    setHorizontalHeaderLabels( QStringList() << "Auftragsname" << "Datum" << "Gestartet"
                               << "Beendet" << "Last log" << "Nachricht"  );
}

void BackupMonitorClientStatusItemModel::clear() {

    QStandardItemModel::clear();
    _insertedGroups.clear();
    _headlineJobLastDate.clear();
}

void BackupMonitorClientStatusItemModel::reloadSearchFilter(int customerId, QString jobFilter, QDate start, QDate end) {

    emit stateLoading("Lade Filter...", 0, 0);

    // Alle Backups aus dem Zeitfilter auslesen:
    CREATE_TCP_COMMAND(getJobs, backupMonitor, getJobs);
    getJobs->setFromDate(start);
    getJobs->setToDate(end);
    getJobs->setCustomerId( customerId );
    getJobs->setJobFilter(jobFilter);
    getJobs->startWait();
    QList< QHash<QString, QVariant> > jobs = getJobs->getReturnedJobs();

    QHash<QString, QVariant> job;
    QStringList customerIds;
    foreach(job, jobs) {

        QString customerId = job["customer_id"].toString();

        if (!customerIds.contains(customerId)) {
            customerIds << customerId;
        }
    }

    // Wurden Jobs gefunden?
    if (customerIds.count() <= 0) return;

    CREATE_TCP_COMMAND(getCustomers, customers, search);
    getCustomers->setReturnColumns( QStringList() << "customer_name" << "id");
    getCustomers->setSearchFilter( customerIds );
    getCustomers->setFilterColumns( QStringList() << "id" );
    getCustomers->startWait();

    while( getCustomers->hasNextCustomer() ) {

        QHash<QString, QVariant> customer = getCustomers->fetchNextCustomer();

        QList<QStandardItem*> items;

        /**
          * Kundenname
          */
        BackupMonitorClientStatusItem *i = new BackupMonitorClientStatusItem(customer["customer_name"].toString(), BackupMonitorClientStatusItem::MAIN_GROUP);
        i->setCustomerId(customer["id"].toInt());

        appendRow(i);

        _insertedGroups[customer["id"].toString()]["-1"] = static_cast<BackupMonitorClientStatusItem*>(i);

        customerIds.removeAll(customer["id"].toString());
    }

    // Nun legen wir noch alle Kunden an, von welche wir keinen Kundennamen finden konnten (Kunde wurde gelöscht??!)
    QString cId;
    foreach(cId, customerIds) {

        BackupMonitorClientStatusItem *i = new BackupMonitorClientStatusItem("NO CUSTOMER NAME FOUND (DELETED???) ID: " + cId, BackupMonitorClientStatusItem::MAIN_GROUP);
        i->setCustomerId(cId.toInt());
        appendRow(i);
        _insertedGroups[cId]["-1"] = static_cast<BackupMonitorClientStatusItem*>(i);
    }

    bool showDateHeadline = start != end;
    foreach(job, jobs) {

        QList<QStandardItem*> items;
        _addJob(job["customer_id"].toString(), job, showDateHeadline);
    }
}

void BackupMonitorClientStatusItemModel::reloadClientStatus(const QDate &date) {

    /**
     * Als ersten fügen wir alle vorhanden Kunden- Gruppen ein:
     */
    _customerGroupsToItems.clear();

    CREATE_TCP_COMMAND(groups, backupMonitor, getCustomerGroups);
    groups->setParameters(QStringList() << "id" << "name" << "auto_expand");
    groups->startWait();
    bool exitInsertCustomerGroups = false;
    int i = 0;
    int totalGroups = groups->total() + 1;      // +1 = Keine Zuordnung
    emit stateLoading("Lade Gruppierungen", 0, totalGroups);
    while (!exitInsertCustomerGroups) {

        QHash<QString, QVariant> group;
        if (!groups->hasNextCustomerGroup()) {
            // Zu aller letzt fügen wir die Gruppe "keine Zuordnung" ein.
            group["name"] = "Keine Zuordnung";
            group["id"] = 0;

            exitInsertCustomerGroups = true;
        }
        else {
            group = groups->fetchNextCustomerGroup();
        }

        QList<QStandardItem*> items;

        /**
          * Gruppenname
          */
        items << new BackupMonitorClientStatusItem(group["name"].toString(), BackupMonitorClientStatusItem::CUSTOMER_GROUP);

        /**
          * 2 leere Items:
          */
        items << new BackupMonitorClientStatusItem("", BackupMonitorClientStatusItem::CUSTOMER_GROUP);
        items << new BackupMonitorClientStatusItem("", BackupMonitorClientStatusItem::CUSTOMER_GROUP);
        items << new BackupMonitorClientStatusItem("", BackupMonitorClientStatusItem::CUSTOMER_GROUP);

        /**
          * Status der Gruppe des heutigen Tages:
          */
        BackupMonitorClientStatusItem *stateOfDay = new BackupMonitorClientStatusItem("", BackupMonitorClientStatusItem::CUSTOMER_GROUP);
        stateOfDay->setItem(BackupMonitorClientStatusItem::ITEM_LAST_BACKUP_DAY);
        items << stateOfDay;

        /**
          * 3 leere Items:
          */
        items << new BackupMonitorClientStatusItem("", BackupMonitorClientStatusItem::CUSTOMER_GROUP);
        items << new BackupMonitorClientStatusItem("", BackupMonitorClientStatusItem::CUSTOMER_GROUP);
        items << new BackupMonitorClientStatusItem("", BackupMonitorClientStatusItem::CUSTOMER_GROUP);


        // Auf alle Items Standard- Werte setzen:
        foreach(QStandardItem *i, items) {
            static_cast<BackupMonitorClientStatusItem*>(i)->setCustomerGroupId(group["id"].toInt());
        }

        appendRow(items);
        _customerGroupsToItems[group["id"].toInt()] = items;

        // Soll die Kundengruppe automatisch expanded werden?
        if (group["id"].toInt() == 0 || group["auto_expand"].toBool()) {
            _view->setExpanded(indexFromItem(items[0]), true);
        }

        emit stateLoading("Lade Gruppierungen", ++i, totalGroups);
    }

    CREATE_TCP_COMMAND(getCustomers, customers, search);
    getCustomers->setReturnColumns( QStringList() << "backup_prospective_jobs_mon" << "backup_prospective_jobs_tue"
                                    << "backup_prospective_jobs_wed" << "backup_prospective_jobs_thu"
                                    << "backup_prospective_jobs_fri" << "backup_prospective_jobs_sat"
                                    << "backup_prospective_jobs_sun" << "customer_name" << "id" << "date_last_log_entry"
                                    << "username_of_last_log_entry"
                                    << "backup_last_hello" << "backup_sync_client_version"  << "backup_group" << "backup_group");
    getCustomers->setSearchFilter("1");
    getCustomers->setFilterColumns( QStringList() << "backup_enabled" );
    getCustomers->startWait();

    QList<QString> daysOfWeek;
    daysOfWeek << "sun" << "mon" << "tue" << "wed" << "thu" << "fri" << "sat" << "sun";

    i = 0;
    int totalCustomers = getCustomers->getCountEntries();
    emit stateLoading("Lade Kunden", 0, totalCustomers);

    while( getCustomers->hasNextCustomer() ) {
        QHash<QString, QVariant> customer = getCustomers->fetchNextCustomer();

        // Wurden Backuptage beim Kunden definiert??
        bool backupDaysDefined;
        if (customer["backup_prospective_jobs_mon"].toInt() < 1 &&
                customer["backup_prospective_jobs_tue"].toInt() < 1 &&
                customer["backup_prospective_jobs_wed"].toInt() < 1 &&
                customer["backup_prospective_jobs_thu"].toInt() < 1 &&
                customer["backup_prospective_jobs_fri"].toInt() < 1 &&
                customer["backup_prospective_jobs_sat"].toInt() < 1 &&
                customer["backup_prospective_jobs_sun"].toInt() < 1) {
            backupDaysDefined = false;
        }
        else {
            backupDaysDefined = true;
        }

        // Den Kunden der entsprechenden Kundengruppe zuordnen:
        QStandardItem* customerGroupItem;
        QStandardItem* customerGroupItemStateOfDay;
        if (_customerGroupsToItems.contains(customer["backup_group"].toInt())) {
            customerGroupItem = _customerGroupsToItems[customer["backup_group"].toInt()][0];
            customerGroupItemStateOfDay = _customerGroupsToItems[customer["backup_group"].toInt()][3];
        }
        else {
            // Wenn keiner Gruppe zugeordnet, oder Gruppe nicht existiert, so in "Keine Zuordnung" einfügen:
            customerGroupItem = _customerGroupsToItems[0][0];
            customerGroupItemStateOfDay = _customerGroupsToItems[0][3];
        }

        QList<QStandardItem*> items;

        /**
          * Kundenname
          */
        items << new BackupMonitorClientStatusItem(customer["customer_name"].toString(), BackupMonitorClientStatusItem::MAIN_GROUP);

        /**
          * Version
          */
        items << new BackupMonitorClientStatusItem(customer["backup_sync_client_version"].toString(), BackupMonitorClientStatusItem::MAIN_GROUP);

        /**
          * Datum des letzten Syncs:
          */
        QDateTime lastSyncTime = QDateTime::fromTime_t(customer["backup_last_hello"].toInt());
        BackupMonitorClientStatusItem *lastSyncItem = new BackupMonitorClientStatusItem(UtilDateTime::getSinceTime(lastSyncTime, false),
                                                                                        BackupMonitorClientStatusItem::MAIN_GROUP);

        // Wenn der letzte Sync > 4 Stunden her ist, so rot einfärben, ansonsten grün...
        bool lastSyncTimeValid = !(lastSyncTime < QDateTime::fromTime_t( QDateTime::currentDateTime().toTime_t() - SECONDS_SYNC_INVALID ) );

        items << lastSyncItem;


        /**
          * Days of last log entry
          */
        int lastLogDays;
        if (customer["date_last_log_entry"].toDate().isValid()) {
            lastLogDays = customer["date_last_log_entry"].toDate().daysTo( QDate::currentDate() );
        }
        else {
            lastLogDays = -1;
        }
        QString lastLogString = QString("%1/%2").arg(QString::number(lastLogDays), customer["username_of_last_log_entry"].toString());

        BackupMonitorClientStatusItem *itemLastLogDays = new BackupMonitorClientStatusItem(lastLogString, BackupMonitorClientStatusItem::MAIN_GROUP);
        //itemLastLogDays->setData(lastLogDays, Qt::UserRole + 2);
        //itemLastLogDays->setItem( BackupMonitorClientStatusItem::ITEM_LAST_LOG_DAYS );
        if (lastLogDays == -1) {
            itemLastLogDays->setForeground( QBrush(QColor(30, 30, 30)) );
            itemLastLogDays->setText("");       // No date available...
        }
        else if (lastLogDays <= 3) {
            itemLastLogDays->setForeground( QBrush(QColor(0, 255, 0)) );
        }
        else if (lastLogDays <= 8) {
            itemLastLogDays->setForeground( QBrush(QColor(255, 215, 0)) );
        }
        else {
            itemLastLogDays->setForeground( QBrush(QColor(255, 0, 0)) );
        }
        items << itemLastLogDays;

        /**
          * Status des letzten Backup- Tages:
          */
        BackupMonitorClientStatusItem *lastBackupReportItem = new BackupMonitorClientStatusItem("", BackupMonitorClientStatusItem::MAIN_GROUP);

        QDate lastBackupDay(date);        // Beinhaltet das Datum des letzten Backuptages
        QHash<QByteArray, QVariant> stateOfLastBackupDay;       // Beinhaltet den Status des letzten Backups
        int prospectiveJobs = -1;   // Erwartende Jobs an diesem Tag
        CREATE_TCP_COMMAND(getStateOfDay, backupMonitor, getBackupStateOfDay);
        if (backupDaysDefined) {



            // Wir laufen nun bis zu maximal 8 Tage rückwärts und schauen, an welchem Tag ein
            // Backup gelaufen sein sollte...
            QHash<QByteArray, QVariant> state;      // Status des letzten Backups
            for (int i = 1; i <= 7; i++) {

                if (customer["backup_prospective_jobs_" + daysOfWeek[lastBackupDay.dayOfWeek()]].toInt() > 0) {

                    // Status des Backup- Tages auslesen:
                    CREATE_TCP_COMMAND(getStateOfDay, backupMonitor, getBackupStateOfDay);
                    getStateOfDay->setCustomerId(customer["id"].toInt());
                    getStateOfDay->setDate( lastBackupDay );
                    getStateOfDay->startWait();
                    stateOfLastBackupDay = getStateOfDay->getNextState();

                    prospectiveJobs = customer["backup_prospective_jobs_" + daysOfWeek[lastBackupDay.dayOfWeek()]].toInt();
                    break;
                }

                lastBackupDay = lastBackupDay.addDays(-1);
            }

            lastBackupReportItem->setText(UtilDateTime::getShortDayOfWeek(lastBackupDay)  + ":");
            lastBackupReportItem->setItem( BackupMonitorClientStatusItem::ITEM_LAST_BACKUP_DAY );

            // Status des letzten Backuptages auslesen:
            getStateOfDay->setCustomerId(customer["id"].toInt());
            getStateOfDay->setDate(lastBackupDay);
            getStateOfDay->startWait();

            int successful = stateOfLastBackupDay["successful"].toInt();
            int warning = stateOfLastBackupDay["warning"].toInt();
            int running = stateOfLastBackupDay["running"].toInt();
            int error = stateOfLastBackupDay["error"].toInt();
            int aborted = stateOfLastBackupDay["aborted"].toInt();

            // Status beim Kunden setzen:
            lastBackupReportItem->setData(successful, Qt::UserRole + 2);
            lastBackupReportItem->setData(warning, Qt::UserRole + 3);
            lastBackupReportItem->setData(running, Qt::UserRole + 4);
            lastBackupReportItem->setData(error, Qt::UserRole + 5);
            lastBackupReportItem->setData(aborted, Qt::UserRole + 6);

            // Status bei der Kunden- Gruppe addieren:
            customerGroupItemStateOfDay->setData( customerGroupItemStateOfDay->data(Qt::UserRole + 2).toInt() + successful, Qt::UserRole + 2);
            customerGroupItemStateOfDay->setData( customerGroupItemStateOfDay->data(Qt::UserRole + 3).toInt() + warning, Qt::UserRole + 3);
            customerGroupItemStateOfDay->setData( customerGroupItemStateOfDay->data(Qt::UserRole + 4).toInt() + running, Qt::UserRole + 4);
            customerGroupItemStateOfDay->setData( customerGroupItemStateOfDay->data(Qt::UserRole + 5).toInt() + error, Qt::UserRole + 5);
            customerGroupItemStateOfDay->setData( customerGroupItemStateOfDay->data(Qt::UserRole + 6).toInt() + aborted, Qt::UserRole + 6);
        }
        else {
            // Es wurden keine Backup- Tage definiert...
            lastBackupReportItem->setText("Backuptage nicht definiert");
        }
        items << lastBackupReportItem;


        /**
          * Status der letzten Backups rückblickend:
          * Wir überprüfen hier alle Tage rückwirkend, auch Tage, welche nicht als Backuptage definiert wurde.
          * Nicht- Backuptage zeigen wir auch nicht an, außer es sind an diesem Tag Jobs gelaufen, so geben wir dem Benutzer einen hinweiß,
          * er solle doch die Konfiguration des Kunden anpassen.
          */
        int backwardLastSuccessfulBackup = 0; // Days of the last succuessful backup...
        bool stopBackwardLastSuccessfulBackup = false;

        BackupMonitorClientStatusItem *retrospectiveItem = new BackupMonitorClientStatusItem("", BackupMonitorClientStatusItem::MAIN_GROUP);
        if (backupDaysDefined) {

            retrospectiveItem->setItem(BackupMonitorClientStatusItem::ITEM_RETROSPECTIVE_BACKUP);

            QDate tmpDate( lastBackupDay );

            int totalShow = 20;     // Wie viele Tage vorraus angezeigt werden sollen.
            retrospectiveItem->setData(totalShow, Qt::UserRole);

            /**
             * Wir ermitteln nun eine Liste mit den Datums, welche Rückblickend angezeigt werden sollen...
             */
            QList< QDate > dates;
            for (int i = 1; i <= totalShow; i++) {

                tmpDate = tmpDate.addDays(-1);

                // Ist dieser Tag ein Backup- Tag?
                if (customer["backup_prospective_jobs_" + daysOfWeek[tmpDate.dayOfWeek()]].toInt() < 1) {
                    i--;
                }
                else {
                    // Dieser Tag war ein Backup- Tag
                    dates.append(tmpDate);
                }
            }

            /**
             * Und nun Fragen wir die Statuse aller Backups in unserer Liste ab:
             */
            CREATE_TCP_COMMAND(getStateOfDayBackward, backupMonitor, getBackupStateOfDay);
            getStateOfDayBackward->setCustomerId(customer["id"].toInt());
            getStateOfDayBackward->setDates(dates);
            getStateOfDayBackward->startWait();


            QHash<QByteArray, QVariant> state;
            int i = 1;
            while (getStateOfDayBackward->hasNextState()) {

                state = getStateOfDayBackward->getNextState();

                QDate dateBackward = state["date"].toDate();

                QString stateChar;
                QString message;
                if (state["error"].toInt() > 0) {
                    stateChar = "r";
                    message = "Jobs mit Fehler abgeschlossen";
                }
                else if (state["aborted"].toInt() > 0) {
                    stateChar = "r";
                    message = "Jobs wurden abgebrochen";
                }
                else if (state["totalWithoutMainJobs"].toInt() < customer["backup_prospective_jobs_" + daysOfWeek[dateBackward.dayOfWeek()]].toInt()) {
                    stateChar = "r";
                    message = QString("Erwartende Jobs %1, gelaufen %2")
                            .arg(QString::number(customer["backup_prospective_jobs_" + daysOfWeek[dateBackward.dayOfWeek()]].toInt()),
                                 QString::number(state["totalWithoutMainJobs"].toInt() ) );
                }
                else if (state["running"].toInt() > 0) {
                    stateChar = "b";
                    message = "Jobs noch nicht abgeschlossen";
                }
                else if (state["warning"].toInt() > 0) {
                    stateChar = "y";
                    message = "Jobs mit Warnungen abgeschlossen";
                }
                else {
                    stateChar = "g";
                    message = "Alles in Ordnung";
                }

                // days of the last successful backup
                if (stateChar == "g" || stateChar == "y" || stateChar == "b") {
                    // We reached a successful / warning backup, stop the cnt.
                    if (!stopBackwardLastSuccessfulBackup) {
                        backwardLastSuccessfulBackup++;
                    }
                    stopBackwardLastSuccessfulBackup = true;
                }
                else if (!stopBackwardLastSuccessfulBackup) {
                    // this day was not successful, uncreate counter
                    backwardLastSuccessfulBackup++;
                }

                // Daten für das Delegate zur Anzeige speichern:
                retrospectiveItem->setData(stateChar,                                   Qt::UserRole + 1 + (i * 7) + 0 );
                retrospectiveItem->setData(dateBackward,                                Qt::UserRole + 1 + (i * 7) + 1 );
                retrospectiveItem->setData(message,                                     Qt::UserRole + 1 + (i * 7) + 2 );
                retrospectiveItem->setData(state["successful"].toInt(),                 Qt::UserRole + 1 + (i * 7) + 3 );
                retrospectiveItem->setData(state["warning"].toInt(),                    Qt::UserRole + 1 + (i * 7) + 4 );
                retrospectiveItem->setData(state["error"].toInt(),                      Qt::UserRole + 1 + (i * 7) + 5 );
                retrospectiveItem->setData(state["aborted"].toInt(),                    Qt::UserRole + 1 + (i * 7) + 6 );

                i++;
            }
        }
        items << retrospectiveItem;

        // Wenn der letzte Log- Eintrag älter ist als das letzte erfolgreiche Backup,
        // so wird die Spalte "last log" ausgeblendet..
        if (backwardLastSuccessfulBackup < lastLogDays) {
            itemLastLogDays->setForeground( QBrush(QColor(30, 30, 30)) );
            itemLastLogDays->setText("");       // No date available...
        }

        /**
          * Anzeigen, an welche Tage gesichert wird..
          */
        // An welche Tage gesichert wird:
        BackupMonitorClientStatusItem *backupDaysItem = new BackupMonitorClientStatusItem("", BackupMonitorClientStatusItem::MAIN_GROUP);
        backupDaysItem->setItem(BackupMonitorClientStatusItem::ITEM_BACKUP_DAYS);
        backupDaysItem->setItemData(customer);
        items << backupDaysItem;

        /**
          * Status:
          */
        BackupMonitorClientStatusItem *stateItem = new BackupMonitorClientStatusItem("", BackupMonitorClientStatusItem::MAIN_GROUP);

        if (!backupDaysDefined) {
            /**
             * Aus übersichtlichkeitsgründen entfernt:
             *
             * stateItem->setIcon( QIcon(":/red.png") );
             * stateItem->setText("Keine Backup- Tage definiert");
             */
        }
        else if (!lastSyncTimeValid) {
            stateItem->setIcon( QIcon(":/red.png") );
            stateItem->setText("Letzte Synchronisierung lange her");
        }
        else if (prospectiveJobs > stateOfLastBackupDay["totalWithoutMainJobs"].toInt()) {
            stateItem->setIcon( QIcon(":/red.png") );
            stateItem->setText( QString("Erwartende Jobs %1, gelaufen %2")
                                .arg(QString::number(prospectiveJobs), QString::number(stateOfLastBackupDay["totalWithoutMainJobs"].toInt()) ) );
        }
        else if (stateOfLastBackupDay["error"].toInt() > 0) {
            stateItem->setIcon( QIcon(":/red.png") );
            stateItem->setText("Jobs mit Fehler abgeschlossen");
        }
        else if (stateOfLastBackupDay["running"].toInt() > 0) {
            stateItem->setIcon( QIcon(":/blue.png") );
            stateItem->setText("Jobs noch nicht beendet");
        }
        else if (stateOfLastBackupDay["aborted"].toInt() > 0) {
            stateItem->setIcon( QIcon(":/purple.png") );
            stateItem->setText("Jobs wurden abgebrochen");
        }
        else if (stateOfLastBackupDay["warning"].toInt() > 0) {
            stateItem->setIcon( QIcon(":/yellow.png") );
            stateItem->setText("Jobs mit Warnungen abgeschlossen");
        }
        else {
            stateItem->setIcon( QIcon(":/green.png") );
            stateItem->setText("Alles in Ordnung");
        }
        items << stateItem;

        // Auf alle Items Standard- Werte setzen:
        foreach(QStandardItem *i, items) {

            static_cast<BackupMonitorClientStatusItem*>(i)->setJobId(-1);
            static_cast<BackupMonitorClientStatusItem*>(i)->setCustomerId(customer["id"].toInt());
        }

        customerGroupItem->appendRow(items);

        //
        _insertedGroups[customer["id"].toString()]["-1"] = static_cast<BackupMonitorClientStatusItem*>(items[0]);

        CREATE_TCP_COMMAND(getJobs, backupMonitor, getJobs);
        getJobs->setFromDate(lastBackupDay);
        getJobs->setToDate(lastBackupDay);
        getJobs->setCustomerId(customer["id"].toInt());
        getJobs->startWait();
        QList< QHash<QString, QVariant> > jobs = getJobs->getReturnedJobs();
        QHash<QString, QVariant> job;
        foreach(job, jobs) {
            _addJob(customer["id"].toString(), job, false);
        }

        emit stateLoading("Lade Kunden", ++i, totalCustomers);
    }
}

void BackupMonitorClientStatusItemModel::_addJob(const QString &groupBy,
                                                 QHash<QString, QVariant> &job, bool showDateHeadline) {

    QString customerId = job["customer_id"].toString();

    /**
      * HEADLINE schreiben, wenn sich das Datum ändert...
      */
    if (_showDateHeadline && showDateHeadline &&
            (!_headlineJobLastDate.contains(customerId) ||
             _headlineJobLastDate[customerId] != job["date"].toDate())) {

        QList<QStandardItem*> items;
        items << new BackupMonitorClientStatusItem(job["date"].toDate().toString(), BackupMonitorClientStatusItem::HEADLINE);
        items << new BackupMonitorClientStatusItem("", BackupMonitorClientStatusItem::HEADLINE);
        items << new BackupMonitorClientStatusItem("", BackupMonitorClientStatusItem::HEADLINE);
        items << new BackupMonitorClientStatusItem("", BackupMonitorClientStatusItem::HEADLINE);
        items << new BackupMonitorClientStatusItem("", BackupMonitorClientStatusItem::HEADLINE);

        _insertedGroups[groupBy]["-1"]->appendRow(items);

        _headlineJobLastDate[customerId] = job["date"].toDate();
    }

    /**
      * Ob Gruppen angezeigt werden sollen oder ausgeblendet werden sollen..
      */
    if (_hideJobGroups) {
        // Es werden keine Software- Gruppen angezeigt...
        job["group_softwareid"] = QString("-1");
    }
    else {

        // Existiert vom Job die Übergeordnete Gruppe schon??
        // Wenn nicht, so legen wir einen "fake" davon an, welcher
        // dann von der Gruppe, sobald diese angelegt wird, überschrieben wird...
        if (job["group_softwareid"].toString() != "-1" &&
                _insertedGroups[groupBy][job["group_softwareid"].toString()] == 0) {

            BackupMonitorClientStatusItem *item =  new BackupMonitorClientStatusItem("PRE-CREATED GROUP " + job["group_softwareid"].toString(), BackupMonitorClientStatusItem::JOB_GROUP);
            _insertedGroups[groupBy][ job["group_softwareid"].toString() ] = item;

            _insertedGroups[groupBy]["-1"]->appendRow( item );
        }
    }

    /**
      * Handelt es sich hierbei um einen Job oder einer Gruppe??
      */
    if (job["type"].toInt() == JOB || job["type"].toInt() == MIGRATION_JOB || job["type"].toInt() == PROTOCOL) {

        /**
          * Es ist ein JOB:
          */

        QList<QStandardItem*> items;

        items << new BackupMonitorClientStatusItem(job["taskname"].toString(), BackupMonitorClientStatusItem::JOB)
              << new BackupMonitorClientStatusItem(job["date"].toDate().toString(), BackupMonitorClientStatusItem::JOB)
              << new BackupMonitorClientStatusItem(job["start_time"].toTime().toString(), BackupMonitorClientStatusItem::JOB);

        // Wenn Job laufend ist, so gibt es keinen Endenden Zeitpunkt:
        if (job["state"].toInt() == 80 || job["state"].toInt() == 6) {
            items << new BackupMonitorClientStatusItem("-", BackupMonitorClientStatusItem::JOB);
        }
        else {
            items << new BackupMonitorClientStatusItem(job["end_time"].toTime().toString(), BackupMonitorClientStatusItem::JOB);
        }

        /**
          * Days of last log entry
          */
        int lastLogDays;
        if (job["date_last_log_entry"].toDate().isValid()) {
            lastLogDays = job["date_last_log_entry"].toDate().daysTo( QDate::currentDate() );
        }
        else {
            lastLogDays = -1;
        }
        // If last log day is older than 20 days, hide days of last log entry
        if (lastLogDays > 19) {
            lastLogDays = -1;
        }
        QString lastLogString;
        if (lastLogDays == 1) {
            lastLogString = "1 day";
        }
        else {
            lastLogString = QString("%1 days").arg(QString::number(lastLogDays));
        }
        BackupMonitorClientStatusItem *itemLastLogDays = new BackupMonitorClientStatusItem(lastLogString, BackupMonitorClientStatusItem::MAIN_GROUP);
        //itemLastLogDays->setData(lastLogDays, Qt::UserRole + 2);
        //itemLastLogDays->setItem( BackupMonitorClientStatusItem::ITEM_LAST_LOG_DAYS );
        if (lastLogDays == -1) {
            itemLastLogDays->setForeground( QBrush(QColor(30, 30, 30)) );
            itemLastLogDays->setText("");       // No date available...
        }
        else if (lastLogDays <= 3) {
            itemLastLogDays->setForeground( QBrush(QColor(0, 255, 0)) );
        }
        else if (lastLogDays <= 8) {
            itemLastLogDays->setForeground( QBrush(QColor(255, 215, 0)) );
        }
        else {
            itemLastLogDays->setForeground( QBrush(QColor(255, 0, 0)) );
        }

        items << itemLastLogDays;

        items << new BackupMonitorClientStatusItem(job["msg"].toString(), BackupMonitorClientStatusItem::JOB);

        // Status- Farbe setzen:
        if (job["state"].toInt() == 0 || job["state"].toInt() == 50) {
            // state 50: Tagesprotokoll
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
        else if (job["state"].toInt() == 3) {
            // Abgebrochen:
            items[0]->setIcon( QIcon(":/purple.png") );
        }
        else {
            // Alles andere wird als Fehler behandelt:
            items[0]->setIcon( QIcon(":/red.png") );
        }

        // Auf alle Items Standard- Werte setzen:
        foreach(QStandardItem *i, items) {
            i->setEditable(false);

            static_cast<BackupMonitorClientStatusItem*>(i)->setJobId(job["id"].toInt());
            static_cast<BackupMonitorClientStatusItem*>(i)->setCustomerId(job["customer_id"].toInt());
            static_cast<BackupMonitorClientStatusItem*>(i)->setItemType(BackupMonitorClientStatusItem::JOB);
        }

        // Job anlegen:
        // Tagesprotokolle werden immer ganz am Beginn eingefügt:
        //if (job["software_id"].toString().startsWith("DP")) {
            // Tagesprotokoll:
        //    _insertedGroups[groupBy][job["group_softwareid"].toString()]->insertRow(0, items );
        //}
        //else {
            _insertedGroups[groupBy][job["group_softwareid"].toString()]->appendRow(items );
        //}

    }
    else if(!_hideJobGroups) {

        /**
          * Es ist eine Gruppe:
          */

        QList<QStandardItem*> items;

        QString taskname;
        if (job["type"].toInt() == MIGRATION_JOB_GROUP) {
            taskname = "MIG_GRP: " + job["taskname"].toString();
        }
        else {
            taskname = job["taskname"].toString();
        }

        // Falls schon eine "PRE CREATED" Gruppe angelegt wurde (weil bereits ein Job im Model angelegt
        // wurde, welcher sich in dieser Gruppe befindet), so bearbeiten wir diesen, ansonsten wird diese
        // Gruppe neu angelegt.
        BackupMonitorClientStatusItem* firstGroupItem = _insertedGroups[groupBy][ job["software_id"].toString( ) ];
        if (firstGroupItem == 0) {
            items << new BackupMonitorClientStatusItem(taskname, BackupMonitorClientStatusItem::JOB_GROUP);
        }
        else {

            firstGroupItem->setText(taskname);
            items << firstGroupItem;
        }


        items << new BackupMonitorClientStatusItem(job["date"].toDate().toString(), BackupMonitorClientStatusItem::JOB_GROUP)
                << new BackupMonitorClientStatusItem(job["start_time"].toTime().toString(), BackupMonitorClientStatusItem::JOB_GROUP);

        if (job["state"].toInt() == 80 || job["state"].toInt() == 6) {
            items << new BackupMonitorClientStatusItem("-", BackupMonitorClientStatusItem::JOB_GROUP);
        }
        else {
            items << new BackupMonitorClientStatusItem(job["end_time"].toTime().toString(), BackupMonitorClientStatusItem::JOB_GROUP);
        }

        items << new BackupMonitorClientStatusItem("", BackupMonitorClientStatusItem::JOB_GROUP);

        items << new BackupMonitorClientStatusItem(job["msg"].toString(), BackupMonitorClientStatusItem::JOB_GROUP);

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
        else if (job["state"].toInt() == 3) {
            // Abgebrochen:
            items[0]->setIcon( QIcon(":/purple.png") );
        }
        else {
            // Alles andere wird als Fehler behandelt:
            items[0]->setIcon( QIcon(":/red.png") );
        }

        // Auf alle Items Standard- Werte setzen:
        foreach(QStandardItem *i, items) {
            i->setEditable(false);

            static_cast<BackupMonitorClientStatusItem*>(i)->setJobId(job["id"].toInt());
            static_cast<BackupMonitorClientStatusItem*>(i)->setCustomerId(job["customer_id"].toInt());
            static_cast<BackupMonitorClientStatusItem*>(i)->setItemType(BackupMonitorClientStatusItem::JOB_GROUP);
        }

        // Falls schon eine "PRE CREATED" Gruppe angelegt wurde (weil bereits ein Job im Model angelegt
        // wurde, welcher sich in dieser Gruppe befindet), so bearbeiten wir diesen, ansonsten wird diese
        // Gruppe neu angelegt.
        if (_insertedGroups[groupBy][job["software_id"].toString()] == 0) {

            // Im Model anlegen:
            _insertedGroups[groupBy]["-1"]->appendRow( items );

            // Im eingefügten Gruppen- Cache anlegen:
            _insertedGroups[groupBy][ job["software_id"].toString() ] = static_cast<BackupMonitorClientStatusItem*>(items[0]);
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

void BackupMonitorClientStatusItemModel::moveCustomerToGroup(quint32 customerId, quint32 groupId) {

    BackupMonitorClientStatusItem* customerItem = _insertedGroups[QString::number(customerId)]["-1"];

    // Ist unter QT4.x.x nicht verfügbar!!
    //moveRow( customerItem->parent()->index(), customerItem->row(),
    //_customerGroupsToItems[groupId]->index(), 0);

    QList<QStandardItem*> itemsToMove = customerItem->parent()->takeRow(customerItem->row());
    _customerGroupsToItems[groupId][0]->appendRow(itemsToMove);

}
