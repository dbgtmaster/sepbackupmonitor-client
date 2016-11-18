#include "BackupMonitorLogWidget.h"
#include "ui_BackupMonitorLogWidget.h"

#include "TcpCommands/backupMonitor/createLog.h"
#include "TcpCommands/backupMonitor/getLog.h"

#include <QDateTime>

#include <QMessageBox>

BackupMonitorLogWidget::BackupMonitorLogWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BackupMonitorLogWidget)
{
    ui->setupUi(this);
}

BackupMonitorLogWidget::~BackupMonitorLogWidget()
{
    delete ui;
}

void BackupMonitorLogWidget::createLog() {

    CREATE_TCP_COMMAND(command, backupMonitor, createLog);
    command->setCustomerId(_customerID);
    command->setTaskName(ui->comboTaskName->currentData().toString());
    command->setLog(ui->inputLog->toPlainText());
    command->startWait();

    ui->inputLog->clear();
    if (ui->comboTaskName->currentData().toString() == "") {
        // Log entry is created for customer and not for the task,
        // so we show all log entries for the customer
        ui->checkBoxShowAllCustomerLogs->setChecked(true);
    }

    initLogEntries();
}

void BackupMonitorLogWidget::setProperties(quint32 customerID, const QString &customerName, const QString &taskName,
                                           bool showLogOnlyForThisTask) {

    _customerID = customerID;
    ui->inputCustomerName->setText(customerName);

    ui->comboTaskName->clear();
    ui->comboTaskName->addItem("Kundenbezogen", "");
    if (!taskName.isEmpty()) {
        ui->comboTaskName->addItem("Job: " + taskName, taskName);
        ui->comboTaskName->setCurrentIndex(1);
    }
    else {
        ui->checkBoxShowAllCustomerLogs->setHidden(true);
    }

    setWindowTitle( QString("Log entries for customer %1").arg(customerName) );

    ui->checkBoxShowAllCustomerLogs->setChecked(showLogOnlyForThisTask);

    connect(ui->pushCreate, SIGNAL(clicked(bool)), this, SLOT(createLog()));
    connect(ui->checkBoxShowAllCustomerLogs, SIGNAL(stateChanged(int)), this, SLOT(initLogEntries()));


    initLogEntries();
}

void BackupMonitorLogWidget::initLogEntries() {


    CREATE_TCP_COMMAND(command, backupMonitor, getLog);
    command->setCustomerID(_customerID);
    if (ui->checkBoxShowAllCustomerLogs->isChecked()) {
        command->setTaskName(ui->comboTaskName->currentData().toString());
    }
    command->startWait();

    QList< QHash<QString, QVariant> > rows = command->getLog();

    show();
    ui->webViewLogs->setStyleSheet("background:transparent");
    ui->webViewLogs->setAttribute(Qt::WA_TranslucentBackground);

    QList< QHash<QString, QVariant> >::iterator it;
    int i = 0;
    QString html;
    html.append("<style type='text/css'>"
                "body {"
                "   padding: 0px;"
                "   margin: 0px;"
                "}"
                "table th {"
                "   text-align: left;"
                "   background-color: #C8C8C8"
                "}"
                "table td {"
                "   background-color: #E8E8E8"
                "}"
                "table td, table th {"
                "    font-size: 12px;"
                "}"
                "</style>");
    html.append("<table width='100%'>");
    html.append("<tr><th>Date</th><th>Username</th><th>Taskname</th><th>Log</th></tr>");
    for (it = rows.begin(); it != rows.end(); it++, i++) {

        QHash<QString, QVariant> row = *it;
        QString dateString;
        if (row["date"].toDate() == QDate::currentDate()) {
            dateString = "today";
        }
        else if (row["date"].toDate() == QDate::currentDate().addDays(-1)) {
            dateString = "yesterday";
        }
        else {
            dateString = row["date"].toDateTime().toString("dd.MM.yyyy");
        }
        html.append("<tr>");
        html.append("<td><nobr>"+ dateString +"</nobr></td>");
        html.append("<td>"+ row["username"].toString().toHtmlEscaped() +"</td>");
        html.append("<td>"+ row["taskname"].toString().toHtmlEscaped() +"</td>");
        html.append("<td>"+ row["log"].toString().toHtmlEscaped() +"</td>");
        html.append("</tr>");

        /*
        QHash<QString, QVariant> row = *it;
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(row["date"].toDateTime().toString("dd.MM.yyyy")));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(row["username"].toString()));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(row["taskname"].toString()));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(row["log"].toString()));*/
    }
    html.append("</table>");
    _logsHtml = html;
    ui->webViewLogs->setHtml(_logsHtml);

    /*
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);*/
}

void BackupMonitorLogWidget::show() {

    QWidget::show();
    ui->webViewLogs->setHtml(_logsHtml);
}
