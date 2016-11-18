/**
  * @author Thomas Manninger
  *
  * Diese Klasse ist Thread- Safe!! Be happy =D
  */

#ifndef TCPMAINTHREAD_H
#define TCPMAINTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QMutex>
#include <QString>
#include <QByteArray>
#include <QTimer>

#include <QSharedPointer>

#include <typeinfo>

class TcpAbstractCommand;

class TcpMainThread : public QThread
{
    Q_OBJECT

private:

    // Singleton- Objekt:
    static TcpMainThread* m_instance;

    // Anzahl MS, die alle abgesetzten Anfragen benötigt haben..
    int _totalMs;

    QTcpSocket _socket;

    // Damit nur über Singleton ein Objekt erzeugt wird:
    explicit TcpMainThread(QObject *parent = 0);
    TcpMainThread(const TcpMainThread &);
    TcpMainThread& operator=(const TcpMainThread &);

    // Erzeugt ein Signal für einen Tcp- Error:
    void emitTcpError(QString message);

    // QTimer sendet alle x Sekunden ein Signal zum Server, um nicht in ein Timeout zu laufen...
    QTimer *_socketTimeoutTimer;

    bool _blockTcpSignalErrors;

public:

    static TcpMainThread* instance();
    static void dropInstance();

    // Baut eine Verbindung zum Tcp- Server auf...
    void connectToHost(const QString& hostname, quint16 port);

    // Beendet die Verbindung zum Tcp- Server...
    void disconnectFromHost();

    // Erstellt einen neuen Tcp- Befehl:
    TcpAbstractCommand* getTcpCommand(const QByteArray& group, const QByteArray& action);

    inline void blockTcpSignalErrors(bool t) { _blockTcpSignalErrors = t; }

    bool isConnected();

    void run();

signals:
    void tcpError(QAbstractSocket::SocketError errId, QString message, QString details);

    void tcpPermissionError(QString group, QString action);
    void tcpUnknownError(QString message);

public slots:

    // Slot wird von der Klasse TcpAbstractCommand aufgerufen, um
    // den Tcp Request zu starten...
    void startTcpCommand();

    // Slot wird aufgerufen, wenn ein TCP- Fehler auftritt...
    // Dieser Slot bereitet die Daten auf und schickt diese der Anwendung weiter...
    void emitTcpError(QAbstractSocket::SocketError);

private slots:

    // Sendet zum TCP- Server ein Aktivitätssignal, damit die Verbindung nach einem Timeout nicht unterbrochen wird...
    void _sendSocketActivity();
};

// Um das erstellen von neuen Befehlen zu kürzen:
inline TcpAbstractCommand* getTcpCommand(const QByteArray& group, const QByteArray& action) {
    return TcpMainThread::instance()->getTcpCommand(group, action);
}

// Um das erstellen eines neuen TcpCommands zu erleichtern:
#define CREATE_TCP_COMMAND(VARIABLE, GROUP, ACTION) \
    QSharedPointer<TcpCommand_ ## GROUP ## _ ## ACTION> VARIABLE( static_cast<TcpCommand_ ## GROUP ## _ ## ACTION*>( getTcpCommand(#GROUP, #ACTION) ) );

#endif // TCPMAINTHREAD_H
