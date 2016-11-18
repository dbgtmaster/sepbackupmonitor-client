#include "BackupMonitorManageGroupsEditor.h"
#include "ui_BackupMonitorManageGroupsEditor.h"

BackupMonitorManageGroupsEditor::BackupMonitorManageGroupsEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BackupMonitorManageGroupsEditor)
{
    ui->setupUi(this);

    connect(ui->inputName, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
    connect(ui->inputDescription, SIGNAL(textChanged()), this, SIGNAL(changed()));
    connect(ui->checkBoxAutoExpand, SIGNAL(clicked()), this, SIGNAL(changed()));
}

BackupMonitorManageGroupsEditor::~BackupMonitorManageGroupsEditor()
{
    delete ui;
}
