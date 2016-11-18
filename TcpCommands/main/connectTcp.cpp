#include "connectTcp.h"

QString TcpCommand_main_connectTcp::_lastHostname;
quint16 TcpCommand_main_connectTcp::_lastPort;

TcpCommand_main_connectTcp::TcpCommand_main_connectTcp(QObject *parent)
    : TcpAbstractCommand(parent), _isError(false)
{
}

void TcpCommand_main_connectTcp::setError(const QString &s) {
    _isError = true;
    _errorString = s;
}

bool TcpCommand_main_connectTcp::isError() {
    return _isError;
}

QString& TcpCommand_main_connectTcp::errorString() {
    return _errorString;
}

void TcpCommand_main_connectTcp::setHostname(const QString hostname) {
    _hostname = hostname;
    _lastHostname = hostname;
}

void TcpCommand_main_connectTcp::setPort(const quint16 port) {
    _port = port;
    _lastPort = port;
}

void TcpCommand_main_connectTcp::useLastHostData() {
    _port = _lastPort;
    _hostname = _lastHostname;
}

void TcpCommand_main_connectTcp::execWriteData() {

    // TCP Fehlerbehandlung:
    connect(TcpMainThread::instance(), SIGNAL(tcpError(QAbstractSocket::SocketError,QString,QString)),
            this, SLOT(tcpError(QAbstractSocket::SocketError,QString,QString)), Qt::DirectConnection);

    // Baut eine Verbindung zum Server auf...
    // Sollte bereits eine Verbindung existieren, so wird diese automatatisch zuvor getrennt...
    TcpMainThread *tcp = TcpMainThread::instance();
    tcp->disconnectFromHost();
    tcp->connectToHost(_hostname, _port);

    // Wenn ein Fehler augetreten ist, so brauchen wir nix mehr tun...
    if (isError()) {
        stopCommandExecution();
        return;
    }

    // Wenn IP Adresse als Zielserver angeben wird, und nicht erreicht werden kann, wird kein error geworfen...
    if (!tcp->isConnected()) {
        setError("Kann keine Verbindung herstellen.");
        stopCommandExecution();
        return;
    }

    // Hello Greez:
    _dataStream << QByteArray("hi");
}

// Da wir keine Daten, können wir diese Methode leer lassen...
void TcpCommand_main_connectTcp::execReadData() {
    // Server muss mit "hello client" antworten...
    QByteArray answer;
    _dataStream >> answer;

    if (answer != "hi") {

        // Fehler beim verbinden aufgetreten...
        // Eventuell handelt es sich hierbei um keinen Intranet- Server??
        setError("Could not connect to intranet tcp server. Server answered with wrong hello answer. Is the destination a Siedl intranet server?");
        emit error();
        return;
    }

    emit successful();
}

// Wenn ein Tcp- Fehler aufgetreten ist...
void TcpCommand_main_connectTcp::tcpError(QAbstractSocket::SocketError errId, QString text, QString details) {
    setError(text);
}
