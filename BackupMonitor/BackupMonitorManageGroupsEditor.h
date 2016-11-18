#ifndef BACKUPMONITORMANAGEGROUPSEDITOR_H
#define BACKUPMONITORMANAGEGROUPSEDITOR_H

#include <QWidget>
#include <QString>

#include "ui_BackupMonitorManageGroupsEditor.h"

namespace Ui {
class BackupMonitorManageGroupsEditor;
}

class BackupMonitorManageGroupsEditor : public QWidget
{
    Q_OBJECT

public:
    explicit BackupMonitorManageGroupsEditor(QWidget *parent = 0);
    ~BackupMonitorManageGroupsEditor();

    inline void setName(const QString &name) { ui->inputName->setText(name); }
    inline QString getName() { return ui->inputName->text(); }
    inline void setDescription(const QString &description) { ui->inputDescription->setPlainText(description); }
    inline QString getDescription() { return ui->inputDescription->toPlainText(); }
    inline void setAutoExpand(const bool autoExpand) { ui->checkBoxAutoExpand->setChecked(autoExpand); }
    inline bool isAutoExpand() { return ui->checkBoxAutoExpand->isChecked(); }

private:
    Ui::BackupMonitorManageGroupsEditor *ui;

signals:
    // Wenn Datum im Editor geändert wurden..
    void changed();

public:
};

#endif // BACKUPMONITORMANAGEGROUPSEDITOR_H
