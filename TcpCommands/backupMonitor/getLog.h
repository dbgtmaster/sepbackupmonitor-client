/**
  * Gibt Informationen zu einem Job zurÃŒck...
  */

#ifndef TCPCOMMAND_BACKUPMONITOR_GETLOG_H
#define TCPCOMMAND_BACKUPMONITOR_GETLOG_H

#include <TcpAbstractCommand.h>

#include <QObject>
#include <QString>
#include <QList>
#include <QHash>

class TcpCommand_backupMonitor_getLog : public TcpAbstractCommand
{
    Q_OBJECT

    quint32 _customerID;
    QString _taskName;

    QList< QHash<QString, QVariant> > _rows;

public:
    
    TcpCommand_backupMonitor_getLog(QObject *parent = 0);
    
    void execWriteData();
    void execReadData();
    
    inline void setCustomerID(quint32 id);

    // Bentötigte Spalten:
    inline void setTaskName(const QString &taskName);

    inline QList< QHash<QString, QVariant> > getLog();
};

void TcpCommand_backupMonitor_getLog::setCustomerID(quint32 id) {
    _customerID = id;
}

void TcpCommand_backupMonitor_getLog::setTaskName(const QString &taskName) {
    _taskName = taskName;
}

QList< QHash<QString, QVariant> > TcpCommand_backupMonitor_getLog::getLog() {
    return _rows;
}

#endif // TCPCOMMAND_BACKUPMONITOR_GETLOG_H


