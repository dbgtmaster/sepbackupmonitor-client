/**
  * Sucht nach bestimmten Filtern Jobs und gibt eine Liste mit den Funden zurÃŒck...
  */

#ifndef TCPCOMMAND_BACKUPMONITOR_GETJOBS_H
#define TCPCOMMAND_BACKUPMONITOR_GETJOBS_H

#include <TcpAbstractCommand.h>

#include <QObject>
#include <QString>
#include <QDate>

class TcpCommand_backupMonitor_getJobs : public TcpAbstractCommand
{
    Q_OBJECT

private:
    QDate _fromDate;
    QDate _toDate;
    quint32 _customerId;
    QString _jobFilter;
    QList<QDate> _backwardDays;

    QList< QHash<QString, QVariant> > _returnedJobs;

public:
    
    TcpCommand_backupMonitor_getJobs(QObject *parent = 0);
    
    void execWriteData();
    void execReadData();

    inline void setFromDate(const QDate &d);
    inline void setToDate(const QDate &d);

    // Wird keine ID oder -1 gesetzt, so werden alle Kunden durchsucht...
    inline void setCustomerId(quint32 id);

    // Setzt einen Filter, ob nur bestimmte Jobs angezeigt werden sollen...
    inline void setJobFilter(QString &f);

    // Tage, welche Rückblickend der Status ermittelt werden soll:
    inline void setBackwardDays(const QList<QDate> dates);

    inline QList< QHash<QString, QVariant> > getReturnedJobs();

};

void TcpCommand_backupMonitor_getJobs::setFromDate(const QDate &d) {
    _fromDate = d;
}

void TcpCommand_backupMonitor_getJobs::setToDate(const QDate &d) {
    _toDate = d;
}

void TcpCommand_backupMonitor_getJobs::setCustomerId(quint32 id) {
    _customerId = id;
}


void TcpCommand_backupMonitor_getJobs::setJobFilter(QString &f) {
    _jobFilter = f;
}

void TcpCommand_backupMonitor_getJobs::setBackwardDays(const QList<QDate> dates) {
    _backwardDays = dates;
}

QList< QHash<QString, QVariant> > TcpCommand_backupMonitor_getJobs::getReturnedJobs() {
    return _returnedJobs;
}

#endif // TCPCOMMAND_BACKUPMONITOR_GETJOBS_H


