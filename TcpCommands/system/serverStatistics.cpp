#include "serverStatistics.h"

TcpCommand_system_serverStatistics::TcpCommand_system_serverStatistics(QObject *parent) :
    TcpAbstractCommand(parent)
{
}

void TcpCommand_system_serverStatistics::execWriteData() {

}

void TcpCommand_system_serverStatistics::execReadData() {

    _dataStream >> _serverData;
}

