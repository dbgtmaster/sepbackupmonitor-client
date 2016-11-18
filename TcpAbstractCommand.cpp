#include "TcpAbstractCommand.h"

#include <QCoreApplication>

#if defined(Q_OS_WIN32)
    #include <windows.h>
#else
    #include <unistd.h>
#endif

// Static members:
QMutex TcpAbstractCommand::_commIdMutex;
quint16 TcpAbstractCommand::_commIdCounter = 0;

TcpAbstractCommand::TcpAbstractCommand(QObject* parent)
    : QObject(parent), _commandDone(false), _dataStream(&_dataStreamArray, QIODevice::WriteOnly), _stopCommandExecution(false), _noTcpExecution(false)
{

    // Da der Server derzeit 4.6 verwendet:
    /**
     * @TODO:
     * Nach QT Update Version hochsetzen:
     */
    _dataStream.setVersion(QDataStream::Qt_4_6);

    // Kommunikations- ID generieren:
    _commIdMutex.lock();

    _commIdCounter++;
    _commId = _commIdCounter;

    // Sind wir mit dem Zähler über 60.000 (max größe: 65,535), so reseten wir diesen wieder auf 0, damit wir
    // nicht über die Grenzen hinauszählen...
    if (_commIdCounter > 60000) _commIdCounter = 0;

    _commIdMutex.unlock();
}

TcpAbstractCommand::~TcpAbstractCommand() {

}

void TcpAbstractCommand::setCommandDone() {
    _commandDone = true;
}

void TcpAbstractCommand::setGroupActionName(const QByteArray &group, const QByteArray &action) {

    _groupName = group;
    _actionName = action;
}

/**
  * Diese Methode sorgt dafür, dass der TcpThread die Methode exec() aufruft...
  * Antwort wird via Signale des TcpCommands mitgeteilt...
  */
void TcpAbstractCommand::start() {

    if (_noTcpExecution) return;

    emit startCalled();
}

/**
  * Diese Methode sorgt dafür, dass der TcpThread die Methode exec() aufruft...
  * Diese Methode blockiert solange, bis die Anfrage abgearbeitet wurde. Es wird aber nicht die
  * Anwendung blockiert, da diese Methode dafür sorgt, dass der Event- Loop in der Wartezeit weiter
  * ausgeführt wird.
  */
void TcpAbstractCommand::startWait(bool blockUserInteractions) {

    if (_noTcpExecution) return;

    start();

    // Nun warten wir solange, bis dieser Befehl abgearbeitet wurde...
    while (true) {

        if (_commandDone) break;

        // 1 MS warten, und dann nochmal schauen, ob dieser Befehl fertig abgearbeitet wurde...
        // qWait sorgt dafür, dass die Event- Loop weiter ausgeführt wird...
        #if defined(Q_OS_WIN32)
            Sleep(2);
        #else
            usleep(2000);
        #endif
        if (blockUserInteractions) {
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        }
        else {
            QCoreApplication::processEvents();
        }
    }
}

void TcpAbstractCommand::stopCommandExecution() {
    _stopCommandExecution = true;
}

bool TcpAbstractCommand::getStopCommandExecution() {
    return _stopCommandExecution;
}

void TcpAbstractCommand::setReadWriteSocket(QTcpSocket *s) {
    _socket = s;
}

// Zum ausführen eines Befehl group/command.
void TcpAbstractCommand::initDataStreamTcpCommand() {

    // quint32 reservieren für die Größe dieses Datenpaketes
    _dataStream << (quint32) 0;

    // Art des Befehl: 0 -> Tcp befehl
    _dataStream << (quint8) 0;

    // quint16 reservieren für die Kommunikations- ID:
    _dataStream << (quint16) _commId;

    // Gruppierung und Actionsname in Datei schreiben...
    _dataStream << QByteArray(_groupName);
    _dataStream << QByteArray(_actionName);

    _tcpType = TYPE_TCP_COMMAND;
}

// Restet den Timeout timer am intranet-server. Weitere Daten sind nicht erforderlich.
void TcpAbstractCommand::initDataStreamTimeoutTimer() {

    // quint32 reservieren für die Größe dieses Datenpaketes
    _dataStream << (quint32) 0;

    // Art des Befehl: 1 ->
    _dataStream << (quint8) 1;

    _tcpType = TYPE_TIMEOUT_TIMER;
}

void TcpAbstractCommand::writeDataStream() {

    // Nun fügen wir die Gesamtlänge der Daten hinzu:
    _dataStream.device()->seek(0);
    _dataStream << (quint32)(_dataStreamArray.size() - sizeof(quint32) );

    qDebug() << "startWrite";
    // Und die Daten lossenden:
    _socket->write(_dataStreamArray);

    _socket->flush();
    qDebug() << "writeFinsihed";
}

// Die Antwort des Tcp- Servers:
void TcpAbstractCommand::waitForAnswer() {

    // Stream leeren...
    _dataStreamArray.clear();

    // Socket leeren, falls von der zuletzt ausgeführten Aktion noch Daten vorhanden sind...
    _socket->readAll();

    // Und den Datenstream auf den Socket umpoolen:     
    _dataStream.setDevice(_socket);

    quint32 dataStreamSize = 0;
    qDebug() << _socket->state();
    while( _socket->waitForReadyRead(30000) ) {

        // Wurde schon bestimmt, wie viele Bytes vom Client erwarten zu sind?
        if (dataStreamSize == 0) {

            // Wurden schon genug Daten empfangen, um die zu erwartenden Bytes zu bestimmen?
            if (_socket->bytesAvailable() <= sizeof(quint32) ) {
                // Wir müssen auf mehr Daten des Clients warten...
                continue;
            }

            // Anzahl erwartende Bytes:
            _dataStream >> dataStreamSize;
        }
        qDebug() << "aaaaaaaaaaaa" << _socket->bytesAvailable() << "==" << dataStreamSize;
        if (_socket->bytesAvailable() == dataStreamSize) {

            if (_tcpType == TYPE_TCP_COMMAND) {

                // Alle Daten des Servers empfangen...
                // Der abgeleitete TcpCommand kann nun die Daten weiter verarbeiten...

                // Kommunikations- ID wird ausgeschnitten, da derzeit nicht benötigt...
                quint16 commId;
                _dataStream >> commId;

                // Ob ein Fehler aufgetreten ist oder eine Normale Antwort dies ist...
                _dataStream >> _responseType;
            }
            else if (_tcpType == TYPE_TIMEOUT_TIMER) {

                _responseType = NORMAL_RESPONSE;
            }
            return;
        }

    }


}
