#include "BackupMonitorExportThread.h"

#include "TcpCommands/backupMonitor/getJobs.h"

#include "BackupMonitorExport.h"
#include "ui_BackupMonitorExport.h"

#include <QPrinter>
#include <QDir>
#include <QTextDocument>

#include <QMutexLocker>

#include "Utils/html.h"

BackupMonitorExportThread::BackupMonitorExportThread(QObject *parent) :
    QThread(parent)
{
    _aborted = false;
}

void BackupMonitorExportThread::setData(QList<QPair<int, QString> > customers, const QDate &dateFrom, const QDate &dateTo, const QString &destDir) {

    _customers = customers;
    _dateFrom = dateFrom;
    _dateTo = dateTo;
    _destDir = destDir;
}

void BackupMonitorExportThread::run() {

    /**
      * Nun lesen wir alle Kunden aus und erstellen nach und nach die Protokolle...
      */

    int customersSelected = _customers.count();

    QDir::setCurrent(_destDir);

    // Anazhl der gesamten Backup- Tage (Tage der beiden Datums * Ausgewählte Kunden)
    int totalProtocolDays = (_dateFrom.daysTo(_dateTo) + 1) * customersSelected;
    int protocolDaysDone = 0;   // Anzahl der Tage, die bereits erledigt wurden..
    QDate dateProcess;          // Bei welchem Datum man sich gerade befindet..
    typedef QPair<int, QString> pair;
    foreach(pair customer, _customers) {

        if (_checkIfAbortReceived()) {
            return;
        }

        QString customerName = customer.second;
        quint32 customerId = customer.first;

        emit stateMessage( QString("Starte Export vom Kunden '%1'").arg(customerName) );


        _htmlContent = "";
        _initExportData(customerId, customerName, _dateFrom, _dateTo);

        dateProcess = _dateFrom;
        // Nun durchlaufen wir alle Tage...
        while (dateProcess <= _dateTo) {

            if (_checkIfAbortReceived()) {
                return;
            }

            _appendNextDay(dateProcess);

            CREATE_TCP_COMMAND(jobs, backupMonitor, getJobs);
            jobs->setFromDate(dateProcess);
            jobs->setToDate(dateProcess);
            jobs->setCustomerId(customerId);
            jobs->startWait();

            /**
              * Status im Protokoll hinzufuegen:
              *

*/
            QList< QHash<QString, QVariant> > jobsList = jobs->getReturnedJobs();
            QHash<QString, QVariant> job;
            int i = 0;
            foreach(job, jobsList) {
                _appendJob(job);
                i++;
            }

            _dayFinished(i);

            dateProcess = dateProcess.addDays(+1);
            protocolDaysDone++;

            emit state(protocolDaysDone, totalProtocolDays);
        }

        _exportDataFinished(customerId, customerName);
    }

    emit stateMessage("Export beendet!");

    return;
}

void BackupMonitorExportThread::stop() {
    QMutexLocker lock(&_mutexAborted);
    _aborted = true;
}

bool BackupMonitorExportThread::_checkIfAbortReceived() {

    QMutexLocker lock(&_mutexAborted);
    if (_aborted) {
        emit stateMessage("Export wurde abgebrochen!");
        return true;
    }

    return false;
}

void BackupMonitorExportThread::_initExportData(quint32 customerId, const QString &customerName, const QDate &startDate, const QDate &endDate) {

    _appendHtml("<html>");
    _appendHtml("   <head>");
    _appendHtml("       <title>Protokoll " + html::htmlspecialchars(customerName) + "</title>");
    _appendHtml("           <style type=\"text/css\">");
    _appendHtml("               .title {");
    _appendHtml("                   background-color: rgb(235, 235, 235);");
    _appendHtml("                   border-style: solid;");
    _appendHtml("                   border-bolor: #000000;");
    _appendHtml("                   font-size: x-large;");
    _appendHtml("                   font-weight: bold;");
    _appendHtml("               }");
    _appendHtml("               .titleDate {");
    _appendHtml("                   background-color: rgb(245, 245, 245);");
    _appendHtml("                   font-size: large;");
    _appendHtml("                   margin-bottom: 10px;");
    _appendHtml("               }");
    _appendHtml("               .jobHeadline {");
    _appendHtml("                   background-color: rgb(166, 166, 166);");
    _appendHtml("                   border-style: solid;");
    _appendHtml("                   border-bolor: #000000;");
    _appendHtml("                   color: #000000;");
    _appendHtml("               }");
    _appendHtml("               .jobTable {");
    _appendHtml("                   margin-top: 0px;");
    _appendHtml("               }");
    _appendHtml("               .jobSuccessful {");
    _appendHtml("                   background-color: rgb(205,255,198);");
    _appendHtml("               }");
    _appendHtml("               .jobWarning {");
    _appendHtml("                   background-color: rgb(254,253,199);");
    _appendHtml("               }");
    _appendHtml("               .jobError {");
    _appendHtml("                   background-color: rgb(255,198,198);");
    _appendHtml("               }");
    _appendHtml("               .jobAborted {");
    _appendHtml("                   background-color: rgb(251,198,255);");
    _appendHtml("               }");
    _appendHtml("               .jobRunning {");
    _appendHtml("                   background-color: rgb(201,198,255);");
    _appendHtml("               }");
    _appendHtml("               .jobNoJobs {");
    _appendHtml("                   background-color: rgb(232,232,232);");
    _appendHtml("               }");
    _appendHtml("           </style>");
    _appendHtml("       </head>");
    _appendHtml("   <body>");
    _appendHtml("       <div class=\"title\">" + html::htmlspecialchars(customerName) + " - Backup Protokolle</div>");
    _appendHtml("       <div class=\"titleDate\">Protokolle von "+ startDate.toString() +" bis "+ endDate.toString() +"</div>");

}

void BackupMonitorExportThread::_appendNextDay(const QDate &date) {

    _appendHtml("<table class=\"jobTable\" width=\"100%\">");
    _appendHtml("	<tr>");
    _appendHtml("       <td colspan=\"3\" class=\"jobHeadline\">Protokolle vom "+ date.toString() +"</td>");
    _appendHtml("	</tr>");
}

void BackupMonitorExportThread::_appendJob(QHash<QString, QVariant> job) {

    QString state;
    int s = job["state"].toInt();
    if (s == 50) return;        // Tagesprotokoll wird ignoriert
    switch(s) {
    case 0:     state = "jobSuccessful";    break;
    case 1:     state = "jobWarning";       break;
    case 80:    state = "jobRunning";       break;
    case 6:     state = "jobRunning";       break;
    case 3:     state = "jobAborted";       break;
    default:
        state = "jobError";

    }

    _appendHtml("	<tr class=\""+ state +"\">");
    _appendHtml("		<td width=\"180\">"+ job["taskname"].toString() +"</td>");
    _appendHtml("		<td width=\"80\">"+ job["drive_name"].toString() +"</td>");
    _appendHtml("		<td>"+ job["msg"].toString() +"</td>");
    _appendHtml("	</tr>");
}

void BackupMonitorExportThread::_dayFinished(int countJobs) {
    if (countJobs == 0) {
        _appendHtml("<tr><td class=\"jobNoJobs\" colspan=\"3\">Keine Jobs</td></tr>");
    }
    _appendHtml("</table>");
}

void BackupMonitorExportThread::_exportDataFinished(quint32 customerId, const QString &customerName) {

    emit stateMessage("Erstelle PDF File...");

    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPrinter::A4);
    printer.setOutputFormat(QPrinter::PdfFormat);

    QString filename = QString("Backup-Protokoll %1.pdf").arg(customerName);


    if (QFile::exists(filename)) {
        emit errorMessage("PDF Datei existiert bereits!!! Diese wird überschrieben!");
    }

    printer.setOutputFileName( filename );

    // Dokument drucken:
    QTextDocument document;
    document.setHtml(_htmlContent);

    // Und Dokument drucken:
    document.print(&printer);

    if (printer.printerState() == QPrinter::Error) {
        emit errorMessage("Fehler beim erstellen des PDFs aufgetreten!");
    }

    /*
     * Wurde zu Testzwecken verwendet...
     *
     *QFile output("Backup-Protokoll " + customerName + ".html");
    output.open(QIODevice::WriteOnly);
    if (!output.isWritable()) {
        emit errorMessage("Kann HTML Protokoll- Datei nicht am Zielpfad schreiben!");
        return;
    }

    output.write(_htmlContent.toUtf8());
    output.close();
    */
}
