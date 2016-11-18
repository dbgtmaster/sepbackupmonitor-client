#include "resetTimeout.h"

TcpCommand_main_resetTimeout::TcpCommand_main_resetTimeout(QObject *parent) :
    TcpAbstractCommand(parent)
{
}

void TcpCommand_main_resetTimeout::execWriteData() {

    _dataStreamArray.clear();
    _dataStream.device()->seek(0);

    // Datenstream umschreiben, um den Timeout am intranet-server zurückzusetzen.
    initDataStreamTimeoutTimer();
}

void TcpCommand_main_resetTimeout::execReadData() {

    quint16 i;
    _dataStream >> i;

    // Es muss ganz genau die Zahl "15253" zurückgegeben werden, ansonsten gibt es ein Problem mit der Tcp- Verbindung...
    if (i != 15253) {
        TcpMainThread::instance()->emitTcpError(QAbstractSocket::ConnectionRefusedError);
    }
}

