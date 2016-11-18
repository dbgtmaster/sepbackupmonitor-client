#include "TcpCommands/backupMonitor/getJobs.h"

#include <QDate>

TcpCommand_backupMonitor_getJobs::TcpCommand_backupMonitor_getJobs(QObject *parent) : TcpAbstractCommand(parent)
{
}

// Daten, welche in den Datenstream geschrieben werden...
// Daten werde in "_dataStream << data" geschrieben...
void TcpCommand_backupMonitor_getJobs::execWriteData() {
    
    _dataStream << _fromDate;
    _dataStream << _toDate;
    _dataStream << _customerId;
    _dataStream << _jobFilter;
    _dataStream << _backwardDays;
}

// Daten, welche der Server uns zurÃŒckgesendet hat...
// Daten werden von "_dataStream >> data" gelesen...
void TcpCommand_backupMonitor_getJobs::execReadData() {
    
    _dataStream >> _returnedJobs;
}

