/**
  * Gibt Informationen zu einem Job zurÃŒck...
  */

#ifndef TCPCOMMAND_BACKUPMONITOR_CREATELOG_H
#define TCPCOMMAND_BACKUPMONITOR_CREATELOG_H

#include <TcpAbstractCommand.h>

#include <QObject>
#include <QString>
#include <QStringList>

class TcpCommand_backupMonitor_createLog : public TcpAbstractCommand
{
    Q_OBJECT

    quint32 _customerID;
    QString _taskName;
    QString _log;

public:
    
    TcpCommand_backupMonitor_createLog(QObject *parent = 0);
    
    void execWriteData();
    void execReadData();
    
    inline void setCustomerId(quint32);
    inline void setTaskName(const QString&);
    inline void setLog(const QString&);
};

void TcpCommand_backupMonitor_createLog::setCustomerId(quint32 id) {
    _customerID = id;
}

void TcpCommand_backupMonitor_createLog::setTaskName(const QString &taskName) {
    _taskName = taskName;
}

void TcpCommand_backupMonitor_createLog::setLog(const QString &log) {
    _log = log;
}

#endif // TCPCOMMAND_BACKUPMONITOR_CREATELOG_H


