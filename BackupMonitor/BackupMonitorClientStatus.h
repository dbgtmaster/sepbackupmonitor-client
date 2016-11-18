#ifndef BACKUPMONITORCLIENTSTATUS_H
#define BACKUPMONITORCLIENTSTATUS_H

#include <QWidget>
#include <QModelIndex>

#include <BackupMonitor/BackupMonitorClientStatusPleaseWaitWidget.h>

namespace Ui {
class BackupMonitorClientStatus;
}

class BackupMonitorClientStatus : public QWidget
{
    Q_OBJECT
    
private:
    bool _searchModeEnabled;

    // Läd die Kundenliste für den Suchfilter neu:
    void _searchModeLoadCustomers();

    // Eine Liste, mit den Gruppen, welche den Kunden zugeordnet werden kann.
    // quuint32 -> Gruppenid, QVariant -> Gruppenname
    QList< QHash<QString, QVariant > > _customerGroups;

    BackupMonitorClientStatusPleaseWaitWidget _pleaseWaitWidget;

public:
    explicit BackupMonitorClientStatus(QWidget *parent = 0, bool searchMode = false);
    ~BackupMonitorClientStatus();

    // Die Suche aktivieren / deaktivieren...
    void enableSearchMode(bool t);

    // Bitte warten Dialog ein & ausblenden:
    void showPleaseWait();
    void hidePleaseWait();

    // Ob zu den Jobs der Kundenname angezeigt bzw. ausgeblendet werden soll.
    void showDateHeadline(bool t);

private:
    // Läd die Liste der verfügbaren Kundengruppen neu..
    void _reloadCustomerGroups();

public slots:

    // Initialisiert das Fenster
    void initClientStatus();
    void initSearchMode();

    // Wenn Benutzer die Maus im treeView bewegt:
    void clientStatusTreeViewEntered(QModelIndex index);

    // Wenn Benutzer mit rechter Maustaste im treeView klickt:
    void clientStatusContextMenuRequested(QPoint);

    // Öffnet das Fenster mit den Kundeninformationen..
    void QActionShowCustomerWindow();

    // Öffnet das Fenster mit den Kunden Log..
    void QActionShowCustomerLogWindow();

    // Öffnet das Fenster für Informationen zu einem Job:
    void QActionShowJobWindow();

    // Zeigt die Jobs der letzten 10 Tage eines Kunden an:
    void QActionShowJobs10DaysAgo();

    // Zeigt den ausgewählten Job den Status der letzten 10 Tage an:
    void QActionShowJob10DaysAgo();

    // Wenn Benutzer für einen Kunde eine andere Kundengruppe ausgewählt hat:
    void QActionCustomerToGroupChanged();

    // Suchfilter neu laden:
    void reloadSearchFilter();

    // Läd den ClientStatus neu:
    void reloadClientState();

    // Wenn ein Job doppelt- Geklickt wird:
    void jobDoubleClicked(QModelIndex);

    void openManageGroupsWindow();

    void openExportWindow();

private:
    Ui::BackupMonitorClientStatus *ui;
};

#endif // BACKUPMONITORCLIENTSTATUS_H
