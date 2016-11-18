/**
  * Gibt Informationen zu einem Job zurÃŒck...
  */

#ifndef TCPCOMMAND_BACKUPMONITOR_GETJOB_H
#define TCPCOMMAND_BACKUPMONITOR_GETJOB_H

#include <TcpAbstractCommand.h>

#include <QObject>
#include <QString>
#include <QStringList>

class TcpCommand_backupMonitor_getJob : public TcpAbstractCommand
{
    Q_OBJECT

    quint32 _jobId;
    QStringList _getCols;

    QHash<QString, QVariant> _row;

public:
    
    TcpCommand_backupMonitor_getJob(QObject *parent = 0);
    
    void execWriteData();
    void execReadData();
    
    inline void setJobId(quint32 id);

    // Bentötigte Spalten:
    inline void setGetCols(const QStringList &cols);

    inline QHash<QString, QVariant> getJob();
};

void TcpCommand_backupMonitor_getJob::setJobId(quint32 id) {
    _jobId = id;
}

void TcpCommand_backupMonitor_getJob::setGetCols(const QStringList &cols) {
    _getCols = cols;
}

QHash<QString, QVariant> TcpCommand_backupMonitor_getJob::getJob() {
    return _row;
}

#endif // TCPCOMMAND_BACKUPMONITOR_GETJOB_H


