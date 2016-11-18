#include "TcpMainThread.h"

#include "TcpCommandFactories.h"

#include <TcpAbstractCommand.h>
#include <QMessageBox>

#include <QTime>

#include <TcpCommands/main/resetTimeout.h>

TcpMainThread* TcpMainThread::m_instance = 0;

/**
  * Gibt das Singleton TcpMainThread Objekt zurück...
  */
TcpMainThread* TcpMainThread::instance() {

    static QMutex mutex;

    // Object anlegen, falls es noch nicht existiert...
    mutex.lock();
    if (m_instance == 0) {
        m_instance = new TcpMainThread();
        m_instance->start();
    }
    mutex.unlock();

    return m_instance;
}

void TcpMainThread::dropInstance() {

    static QMutex mutex;
    mutex.lock();

    if (m_instance != 0) {
        delete m_instance;
        m_instance = 0;
    }

    mutex.unlock();
}

TcpMainThread::TcpMainThread(QObject *parent) :
    QThread(parent)
{
    moveToThread(this);

    _totalMs = 0;

    _blockTcpSignalErrors = false;

    // TcpSocket in diesen Thread verschrieben:
    _socket.moveToThread(this);

    // Tcp- Fehler als Signal der Anwendung weiterleiten...
    connect(&_socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(emitTcpError(QAbstractSocket::SocketError)), Qt::DirectConnection);
}

// Baut eine Verbindung zum Server auf:
void TcpMainThread::connectToHost(const QString &hostname, quint16 port) {

    disconnectFromHost();
    qDebug() << "1" << _socket.state();
    _socket.connectToHost(hostname, port);

    qDebug() << "2" << _socket.state();
    // Timeout von 8 Sekunden...
    if (! _socket.waitForConnected(5000)) {

        qDebug() << "3" << _socket.state();

        // Fehler beim Verbinden zum Server ist aufgetreten...
        // Fehler brauchen wir hier keinen produzieren, da hier sowieso das Signal "error" von QTcpSocket
        // geworfen wird...

        return;
    }
}

void TcpMainThread::disconnectFromHost() {
    _socket.disconnectFromHost();
}

bool TcpMainThread::isConnected() {
    return (_socket.state() == QTcpSocket::ConnectedState);
}

// Sendet zum SEP backupserver ein Paket, den Timer zu resten..
void TcpMainThread::_sendSocketActivity() {

    // Nur, wenn wir verbunden sind, ansonsten hat es auch wenig Sinn :)...
    if (_socket.state() != QAbstractSocket::ConnectedState) {
        return;
    }

    CREATE_TCP_COMMAND(timer, main, resetTimeout);
    timer->start();
}

TcpAbstractCommand* TcpMainThread::getTcpCommand(const QByteArray &group, const QByteArray &action) {

    // Command holen:
    TcpAbstractCommand* tcpCommand( TcpCommandFactories::instance()->getCommand(group, action) );

    qDebug() << "Run action " << group << action;

    // Wurde 0 zurückgegeben, so existiert dieser Befehl nicht:
    if (tcpCommand == 0) {
        qFatal( "TcpMainThread::createTcpCommand(): TcpCommand existiert nicht!" );
    }

    connect(tcpCommand, SIGNAL(startCalled()), this, SLOT(startTcpCommand()));

    return tcpCommand;
}

/**
  * @slot
  *
  * !!!!!!!!!!
  *  ACHTUNG: Dieser Slot darf nur über Klassen aufgerufen, welche von der Klasse
  *  TcpAbstractCommand geerbt wurden, ansonsten sind Programmabstürze die folgen!!!
  * !!!!!!!!!!
  */
void TcpMainThread::startTcpCommand() {


    // Wir supporten derzeit maximal einen gleichzeitigen Request, daher müssen alle anderen warten...
    static QMutex mutex;
    QMutexLocker locker(&mutex);
    QTime timeStart( QTime::currentTime() );

    TcpAbstractCommand* command = static_cast<TcpAbstractCommand*>(sender());
    command->setReadWriteSocket(&_socket);
    // Datenstream initialisieren...
    // Setzt die reservierten Bytes für die übertragene Größe, KommunikationsId,...
    command->initDataStreamTcpCommand();
    // Nun die zuschreibenden Daten aufbereiten lassen
    command->execWriteData();
    if (!command->getStopCommandExecution()) {

        // Und den Stream lossenden:
        command->writeDataStream();

        // Nun warten wir, bis der Server uns vollständig die Antwort gesendet hat...
        command->waitForAnswer();

        // Wir reseten den Timeout- Timer.
        _socketTimeoutTimer->stop();
        _socketTimeoutTimer->start();

        qDebug() << "time to wait for answer:" << timeStart.msecsTo(QTime::currentTime()) << "ms";

        // Überprüfen, ob ein TCP Berechtigungsfehler aufgetreten ist...?
        if (command->getResponseType() == TcpAbstractCommand::TCP_PERMISSION_ERROR) {

            emit tcpPermissionError( command->getGroupName(), command->getActionName());
        }
        else if (command->getResponseType() == TcpAbstractCommand::NORMAL_RESPONSE) {
            // Zuletzt verarbeiten wir noch die erhaltenen Daten des Servers.
            command->execReadData();
        }
        else {
            // Unbekannter Fehler aufgetreten...:
            emit tcpError(QAbstractSocket::UnknownSocketError,
                          QString("Aktion %1/%2 konnte Aufgrund eines unbekannten Kommunikationsfehlers nicht ausgeführt werden.").arg(command->getGroupName(),
                                                                                                                                      command->getActionName()),
                          ""
                                  );
        }
    }

    // Setzen, dass der Befehl abgearbeitet wurde (wird für command->startWait() benötigt)
    command->setCommandDone();

    int ms = timeStart.msecsTo(QTime::currentTime());
    qDebug() << "total time:" << ms << "ms";
    _totalMs += ms;
    qDebug() << "all queries time: " << _totalMs;
}

/**
  * @slot
  *
  * Wird bei einem Fehlerfall der TCP- Verbindung aufgerufen.
  */
void TcpMainThread::emitTcpError(QAbstractSocket::SocketError errId) {

    if (_blockTcpSignalErrors) return;

    QString message;

    switch (errId) {
    case 0:
        message = "The connection was refused by the peer (or timed out).";
        break;
    case 1:
        message = "The remote host closed the connection.";
        break;
    case 2:
        message = "The host address was not found.";
        break;
    case 3:
        message = "The socket operation failed because the application lacked the required privileges.";
        break;
    case 4:
        message = "The local system ran out of resources (e.g., too many sockets).";
        break;
    case 5:
        message = "The socket operation timed out.";
        break;
    case 6:
        message = "The datagram was larger than the operating system's limit.";
        break;
    case 7:
        message = "An error occurred with the network (e.g., the network cable was accidentally plugged out).";
        break;
    // 8 and 9 only udp...
    case 10:
        message = "The requested socket operation is not supported by the local operating system (e.g., lack of IPv6 support).";
        break;
    case 11:
        message = "Used by QAbstractSocketEngine only, The last operation attempted has not finished yet (still in progress in the background).";
        break;
    case 12:
        message = "The socket is using a proxy, and the proxy requires authentication.";
        break;
    case 13:
        message = "The SSL/TLS handshake failed, so the connection was closed.";
        break;
    case 14:
        message = "Could not contact the proxy server because the connection to that server was denied.";
        break;
    case 15:
        message = "The connection to the proxy server was closed unexpectedly (before the connection to the final peer was established).";
        break;
    case 16:
        message = "The connection to the proxy server timed out or the proxy server stopped responding in the authentication phase.";
        break;
    case 17:
        message = "The proxy address was not found.";
        break;
    case 18:
        message = "The connection negotiation with the proxy server because the response from the proxy server could not be understood.";
        break;
    case -1:
        message = "An unidentified error occurred.";
        break;
    default:
        message = QString("TcpMainThread::emitTcpError(QString &message) have not defined the errorId QAbstractSocket::SocketError -> %1")
                            .arg( QString::number(errId) );
    }

    QString details;

    /**
      * @TODO QString details
      * Details der letzten Befehle,....
      */

    emit tcpError(errId, message, details);
}

/**
  * Einstiegspunkt des Tcp Threads
  */
void TcpMainThread::run() {

    // Timeout- Timer: Wir senden alle 60 Sekunden ein Paket zum Intranet-server.
    _socketTimeoutTimer = new QTimer(this);
    _socketTimeoutTimer->moveToThread(this);
    _socketTimeoutTimer->setInterval(60000);
    connect(_socketTimeoutTimer, SIGNAL(timeout()), this, SLOT(_sendSocketActivity()));

    // Event-Loop starten:
    exec();
}
