#ifndef BACKUPMONITORMANAGEGROUPS_H
#define BACKUPMONITORMANAGEGROUPS_H

#include <QWidget>

namespace Ui {
class BackupMonitorManageGroups;
}

class BackupMonitorManageGroups : public QWidget
{
    Q_OBJECT
    
    // Wenn auf true, so pasiert beim Anklicken einer Gruppe im Menü nichts..
    // Hat den Sinn, wenn eine Gruppe gelöscht wird, so wird diese Variable auf
    // true gesetzt, da ansonsten beim löschen die gelöschte Gruppe bearbeitet
    // werden möchte (da das ITEM aus dem ListWidget gelöscht wird -> wirft Signal,
    // dass Selektierung geändert wurde).
    bool _disableEditGroup;

    bool _groupsChanged;

public:
    explicit BackupMonitorManageGroups(QWidget *parent = 0);
    ~BackupMonitorManageGroups();
    
    // Wenn Gruppen bearbeitet wurden, wird true zurückgegeben, ansonsten false..
    bool groupsChanged();
private:
    Ui::BackupMonitorManageGroups *ui;

    void _reloadGroups();

    // Aktiviert, bzw. deaktiviert die UP/DOWN Buttons für die Gruppen
    void _refreshUpDownButtons();

public slots:
    void createNewGroup();
    void startEditGroup();
    void doSaveEditedGroup();
    void enableSaveButton();
    void deleteGroup();

    void groupUp();
    void groupDown();
};

#endif // BACKUPMONITORMANAGEGROUPS_H
