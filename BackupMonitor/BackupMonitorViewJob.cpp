#include "BackupMonitorViewJob.h"
#include "ui_BackupMonitorViewJob.h"

#include "TcpCommands/backupMonitor/getJob.h"
#include "TcpCommands/backupMonitor/getJobProtocol.h"
#include "BackupMonitor/BackupMonitorGlobals.h"

#include <QDate>
#include <QFile>


BackupMonitorViewJob::BackupMonitorViewJob(quint32 jobId, QWidget *parent) :
    QWidget(parent), _jobId(jobId),
    ui(new Ui::BackupMonitorViewJob)
{
    ui->setupUi(this);

    CREATE_TCP_COMMAND(job, backupMonitor, getJob);
    job->setJobId(jobId);
    job->setGetCols( QStringList() << "customer_name" << "customer_id" << "software_id" << "taskname" << "state"
                     << "msg" << "start_time" << "end_time" << "date" << "backup_software" << "type"
                     << "data_size" << "throughput" << "job_type" << "source_list" << "exclude_list" << "drive_name" << "drive_type" << "drive_client_name" );
    job->startWait();
    QHash<QString, QVariant> row = job->getJob();

    /**
      * Je nach Type gewisse Daten anzeigen:
      */
    switch (row["type"].toInt()) {
    case PROTOCOL:
        _removeTab("tabPreProtocol");
        _removeTab("tabPostProtocol");
        _removeTab("tabOverviewJob");

        setWindowTitle( "Tagesprotokoll - " + windowTitle() );
        break;
    case MIGRATION_JOB:
        _removeTab("tabPreProtocol");
        _removeTab("tabPostProtocol");

        setWindowTitle( "Migrationsauftrag - " + windowTitle() );
        break;
    case JOB:
        setWindowTitle( "Auftrag - " + windowTitle() );
    }

    ui->tabWidget->setCurrentIndex(0);

    /** Overview Tab **/
    ui->labelCustomerName->setText( "<b>" + row["customer_name"].toString() + "</b>" );
    ui->labelJobName->setText( "<b>" + row["taskname"].toString() + "</b>" );
    ui->labelDate->setText( row["date"].toDate().toString() );
    ui->labelStartTime->setText( row["start_time"].toTime().toString() );
    ui->labelEndTime->setText( row["end_time"].toTime().toString() );
    ui->labelMessage->setText( row["msg"].toString() );
    ui->labelBackupSoftware->setText( row["backup_software"].toString() );
    ui->labelBackupSoftwareId->setText( row["software_id"].toString() );
    ui->labelJobType->setText( row["job_type"].toString() );

    ui->textEditSourceList->setText( row["source_list"].toString() );
    ui->textEditExcludeList->setText( row["exclude_list"].toString() );

    ui->labelDestinationDrive->setText( row["drive_name"].toString() + " ("+ row["drive_type"].toString() +", rds: "+ row["drive_client_name"].toString() +")" );

    ui->labelDataSize->setText( QString::number( row["data_size"].toFloat() / 1024.0 / 1024.0 / 1024.0, 'f', 2 ) + "GB" );
    ui->labelThroughput->setText( QString::number( row["throughput"].toFloat() / 1024.0, 'f', 2 ) + "GB/h" );

    /** Log Tab **/
    ui->widgetLog->setProperties(row["customer_id"].toInt(), row["customer_name"].toString(), row["taskname"].toString());

    /** Protocal pages **/

    ui->sliderProtocolPage->setValue(1);
    ui->sliderProtocolPage->setMinimum(1);
    ui->sliderProtocolPrePage->setValue(1);
    ui->sliderProtocolPrePage->setMinimum(1);
    ui->sliderProtocolPostPage->setValue(1);
    ui->sliderProtocolPostPage->setMinimum(1);

    connect(ui->pushProtocolBackward, SIGNAL(clicked()), this, SLOT(protocolBackward()));
    connect(ui->pushProtocolForward, SIGNAL(clicked()), this, SLOT(protocolForward()));
    connect(ui->sliderProtocolPage, SIGNAL(sliderReleased()), this, SLOT(protocolSliderChanged()));

    connect(ui->pushProtocolPreBackward, SIGNAL(clicked()), this, SLOT(protocolPreBackward()));
    connect(ui->pushProtocolPreForward, SIGNAL(clicked()), this, SLOT(protocolPreForward()));
    connect(ui->sliderProtocolPrePage, SIGNAL(sliderReleased()), this, SLOT(protocolPreSliderChanged()));

    connect(ui->pushProtocolPostBackward, SIGNAL(clicked()), this, SLOT(protocolPostBackward()));
    connect(ui->pushProtocolPostForward, SIGNAL(clicked()), this, SLOT(protocolPostForward()));
    connect(ui->sliderProtocolPostPage, SIGNAL(sliderReleased()), this, SLOT(protocolPostSliderChanged()));

    reloadProtocol(1);
    reloadProtocolPre(1);
    reloadProtocolPost(1);

    // Zeilenumbruch im Protokoll aktivieren / deaktivieren:
    connect(ui->checkBoxProtocolWordWrap, SIGNAL(toggled(bool)), this, SLOT(setProtocolWordWrap(bool)));
    connect(ui->checkBoxProtocolPreWordWrap, SIGNAL(toggled(bool)), this, SLOT(setProtocolPreWordWrap(bool)));
    connect(ui->checkBoxProtocolPostWordWrap, SIGNAL(toggled(bool)), this, SLOT(setProtocolPostWordWrap(bool)));
    setProtocolWordWrap(ui->checkBoxProtocolWordWrap->isChecked());
    setProtocolPreWordWrap(ui->checkBoxProtocolWordWrap->isChecked());
    setProtocolPostWordWrap(ui->checkBoxProtocolWordWrap->isChecked());
}

BackupMonitorViewJob::~BackupMonitorViewJob()
{
    delete ui;
}

// Aktiviert bzw. deaktiviert den automatischen Zeilenumbruch der Protokollanzeige:
void BackupMonitorViewJob::setProtocolWordWrap(bool b) {

    if (b) {
        ui->textProtocol->setWordWrapMode( QTextOption::WordWrap );
    }
    else {
        ui->textProtocol->setWordWrapMode( QTextOption::NoWrap );
    }
}
void BackupMonitorViewJob::setProtocolPostWordWrap(bool b) {

    if (b) {
        ui->textProtocolPre->setWordWrapMode( QTextOption::WordWrap );
    }
    else {
        ui->textProtocolPre->setWordWrapMode( QTextOption::NoWrap );
    }
}
void BackupMonitorViewJob::setProtocolPreWordWrap(bool b) {

    if (b) {
        ui->textProtocolPost->setWordWrapMode( QTextOption::WordWrap );
    }
    else {
        ui->textProtocolPost->setWordWrapMode( QTextOption::NoWrap );
    }
}

void BackupMonitorViewJob::protocolBackward() {
    reloadProtocol(_protocolPage - 1);
}

void BackupMonitorViewJob::protocolForward() {
    reloadProtocol(_protocolPage + 1);
}

void BackupMonitorViewJob::protocolPreBackward() {
    reloadProtocolPre(_protocolPrePage - 1);
}

void BackupMonitorViewJob::protocolPreForward() {
    reloadProtocolPre(_protocolPrePage + 1);
}

void BackupMonitorViewJob::protocolPostBackward() {
    reloadProtocolPost(_protocolPostPage - 1);
}

void BackupMonitorViewJob::protocolPostForward() {
    reloadProtocolPost(_protocolPostPage + 1);
}


void BackupMonitorViewJob::reloadProtocol(int page) {

    _protocolPage = page;

    CREATE_TCP_COMMAND(getProtocol, backupMonitor, getJobProtocol);
    getProtocol->setJobId(_jobId);
    getProtocol->setProtocolType(TcpCommand_backupMonitor_getJobProtocol::MAIN);
    getProtocol->setProtocolPage(page);
    getProtocol->startWait();

    QString protocolText = getProtocol->getProtocol();

    /**
      * Fehler im Protokoll mit rot bzw. Warnungen mit gelb hervorheben:
      */
    QStringList lines = protocolText.split("\n");
    QStringList::Iterator it;
    for (it = lines.begin(); it != lines.end(); it++) {

        if ( (*it).contains("Warning:") ) {
            (*it) = "<font style=\"background-color: #FFFF00\">" + (*it) + "</font>";
        }

        if ( (*it).contains("Error:") ) {
           (*it) = "<font style=\"background-color: #FA5858\">" + (*it) + "</font>";
        }
    }

    // Label: Seite x/y
    ui->labelProtocolPages->setText( QString("Seite %1/%2").arg(QString::number(page),
                                                                QString::number(getProtocol->getMaxPages())) );
    // Slider positionieren
    ui->sliderProtocolPage->setMaximum(getProtocol->getMaxPages());
    ui->sliderProtocolPage->setValue(page);

    ui->pushProtocolForward->setDisabled( (page >= getProtocol->getMaxPages()) );
    ui->pushProtocolBackward->setDisabled( page <= 1 );

    ui->textProtocol->clear();
    ui->textProtocol->appendHtml(lines.join("<br>"));
    ui->textProtocol->moveCursor(QTextCursor::Start);
}

void BackupMonitorViewJob::protocolSliderChanged() {
    reloadProtocol(ui->sliderProtocolPage->value());
}

void BackupMonitorViewJob::protocolPreSliderChanged() {
    reloadProtocolPre(ui->sliderProtocolPrePage->value());
}

void BackupMonitorViewJob::protocolPostSliderChanged() {
    reloadProtocolPost(ui->sliderProtocolPostPage->value());
}

void BackupMonitorViewJob::reloadProtocolPre(int page) {

    _protocolPrePage = page;

    CREATE_TCP_COMMAND(getProtocol, backupMonitor, getJobProtocol);
    getProtocol->setJobId(_jobId);
    getProtocol->setProtocolType(TcpCommand_backupMonitor_getJobProtocol::PRE);
    getProtocol->setProtocolPage(page);
    getProtocol->startWait();

    QString protocolText = getProtocol->getProtocol();

    /**
      * Fehler im Protokoll mit rot bzw. Warnungen mit gelb hervorheben:
      */
    QStringList lines = protocolText.split("\n");
    QStringList::Iterator it;
    for (it = lines.begin(); it != lines.end(); it++) {

        if ( (*it).contains("Warning:") ) {
            (*it) = "<font style=\"background-color: #FFFF00\">" + (*it) + "</font>";
        }

        if ( (*it).contains("Error:") ) {
           (*it) = "<font style=\"background-color: #FA5858\">" + (*it) + "</font>";
        }
    }

    // Label: Seite x/y
    ui->labelProtocolPrePages->setText( QString("Seite %1/%2").arg(QString::number(page),
                                                                QString::number(getProtocol->getMaxPages())) );
    // Slider positionieren
    ui->sliderProtocolPrePage->setMaximum(getProtocol->getMaxPages());
    ui->sliderProtocolPrePage->setValue(page);

    ui->pushProtocolPreForward->setDisabled( (page >= getProtocol->getMaxPages()) );
    ui->pushProtocolPreBackward->setDisabled( page <= 1 );

    ui->textProtocolPre->clear();
    ui->textProtocolPre->appendHtml(lines.join("<br>"));
    ui->textProtocolPre->moveCursor(QTextCursor::Start);
}

void BackupMonitorViewJob::reloadProtocolPost(int page) {

    _protocolPostPage = page;

    CREATE_TCP_COMMAND(getProtocol, backupMonitor, getJobProtocol);
    getProtocol->setJobId(_jobId);
    getProtocol->setProtocolType(TcpCommand_backupMonitor_getJobProtocol::POST);
    getProtocol->setProtocolPage(page);
    getProtocol->startWait();

    QString protocolText = getProtocol->getProtocol();

    /**
      * Fehler im Protokoll mit rot bzw. Warnungen mit gelb hervorheben:
      */
    QStringList lines = protocolText.split("\n");
    QStringList::Iterator it;
    for (it = lines.begin(); it != lines.end(); it++) {

        if ( (*it).contains("Warning:") ) {
            (*it) = "<font style=\"background-color: #FFFF00\">" + (*it) + "</font>";
        }

        if ( (*it).contains("Error:") ) {
           (*it) = "<font style=\"background-color: #FA5858\">" + (*it) + "</font>";
        }
    }

    // Label: Seite x/y
    ui->labelProtocolPostPages->setText( QString("Seite %1/%2").arg(QString::number(page),
                                                                QString::number(getProtocol->getMaxPages())) );
    // Slider positionieren
    ui->sliderProtocolPostPage->setMaximum(getProtocol->getMaxPages());
    ui->sliderProtocolPostPage->setValue(page);

    ui->pushProtocolPostForward->setDisabled( (page >= getProtocol->getMaxPages()) );
    ui->pushProtocolPostBackward->setDisabled( page <= 1 );

    ui->textProtocolPost->clear();
    ui->textProtocolPost->appendHtml(lines.join("<br>"));
    ui->textProtocolPost->moveCursor(QTextCursor::Start);
}

void BackupMonitorViewJob::_removeTab(const QString &tabObjectName) {

    int tabId = -1;
    QWidget *tabWidget = 0;
    for (int i = 0; i < ui->tabWidget->count(); i++) {
        if (ui->tabWidget->widget(i)->objectName() == tabObjectName) {
            tabId = i;
            tabWidget = ui->tabWidget->widget(i);
        }
    }

    if (tabId == -1) {
        return;
    }

    ui->tabWidget->removeTab(tabId);
    tabWidget->deleteLater();

}
