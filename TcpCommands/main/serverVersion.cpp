#include "serverVersion.h"

TcpCommand_main_serverVersion::TcpCommand_main_serverVersion(QObject *parent) :
    TcpAbstractCommand(parent)
{
}

void TcpCommand_main_serverVersion::execWriteData() {

}

void TcpCommand_main_serverVersion::execReadData() {

    _dataStream >> _serverVersion;
}

QByteArray& TcpCommand_main_serverVersion::getServerVersion() {
    return _serverVersion;
}
