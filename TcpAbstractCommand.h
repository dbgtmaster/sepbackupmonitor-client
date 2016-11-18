#ifndef TCPABSTRACTCOMMANDA_H
#define TCPABSTRACTCOMMANDA_H

#include <QObject>
#include <QDataStream>
#include <QByteArray>
#include <QTcpSocket>
#include <TcpMainThread.h>

#include <QMutex>

#include <TcpCommandsHelper.h>

typedef QMap<QString, QVariant> mapField;
typedef QHash<QString, QVariant> mapHash;

class TcpAbstractCommand : public QObject
{
    Q_OBJECT

public:
    enum RESPONSE_TYPE {
        NORMAL_RESPONSE = 0,
        TCP_PERMISSION_ERROR = 1
    };

private:

    // Socket, von welchem gelesen bzw. geschrieben werden soll...
    QTcpSocket* _socket;

    // Die KommunikationsID...
    // Wird vom Konstruktor dieser Methode generiert...
    quint16 _commId;

    // Damit bei gleichzeitigen Aufruf nicht 2x die gleiche commId übernommen wird...
    static QMutex _commIdMutex;
    static quint16 _commIdCounter;

    // Standardmäßig auf false, wird auf true gesetzt, sobald der Befehl abgearbeitet wurde...
    // Wird für startWait() benötigt...
    bool _commandDone;

    // Soll der READ- Bereich ausgeführt werden?
    bool _stopCommandExecution;

    quint32 _responseType;

    // Wenn auf true gesetzt, so wird keine TCP Abfrage beim starten durchgeführt (Wenn zB. ein gecachter Wert verwendet werden soll)
    bool _noTcpExecution;

    enum _typTypes {
        TYPE_TCP_COMMAND,       // Normaler Befehl group/command
        TYPE_TIMEOUT_TIMER      // Setzt den Timeout Timer am Server zurück..
    };

    _typTypes _tcpType;

protected:

    // Stream Daten, welche zum Server gesendet werden sollen.
    // Benutzt hierfür werden können beide Eigenschaften, da diese miteinander
    // vom Konstruktor verbunden werden...
    // Reihenfolge dieser 2 Membern nicht ändern!
    QByteArray _dataStreamArray;
    QDataStream _dataStream;

    QByteArray _groupName;
    QByteArray _actionName;

public:
    explicit TcpAbstractCommand(QObject* parent = 0);
    ~TcpAbstractCommand();

    // Wird aufgerufen, sobald dieser Befehl fertig abgearbeitet wurde...
    void setCommandDone();

    void setGroupActionName(const QByteArray& group, const QByteArray& action);

    // Teilt dem TcpThread mit, dieses Objekt abzuarbeiten...
    // Die Antwort wird über Signale der abgeleiteten Klasse mitgeteilt.
    void start();

    // Teilt dem TcpThread mit, dieses Objekt abzuarbeiten...
    // Diese Methode blockiert solange, bis die Anfrage abgearbeitet wurde. Es wird aber nicht die
    // Anwendung blockiert, da diese Methode dafür sorgt, dass der Event- Loop in der Wartezeit weiter
    // ausgeführt wird.
    // @param bool blockUserInteractions: Wenn true, dann kann während des Wartens keine Aktion vom User ausgeführt werden (Aktionen werden geblockt,
    //                                    bis startWait verlassen wurde. @see QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    void startWait(bool blockUserInteractions = true);

    // Initialisiert den Datenstream TcpCommand "group/action" mit den Grundinformationen (Reservieren der Bytes für die
    // Größe des Datenpaketes, Kommunikations- ID, Name des Befehls,...)
    void initDataStreamTcpCommand();

    // Restet den Timeout timer am intranet-server.
    void initDataStreamTimeoutTimer();

    void setReadWriteSocket(QTcpSocket *s);

    // Schickt die Daten im Stream zum Tcp- Server:
    // Gruppen, sowie Aktionsname wird automatisch übertragen und muss in dieser Methode
    // nicht angegeben werden...
    void writeDataStream();

    // Diese Methode blockiert so lange, bis die vollständige Antwort des Servers vorhanden ist.
    // Die Antwort wird in den Member _dataStream bzw. _dataStreamArray geschrieben.
    void waitForAnswer();

    // Methode wird von der ableitenden Klasse überschrieben.
    // Diese Methode bereitet die zu übertragenen Daten vor und schickt diese ab.
    virtual void execWriteData() = 0;

    // Methode wird von der ableitenden Klasse überschrieben.
    // Methode wird augerufen, sobald eine Antwort vom Server verfügbar ist.
    virtual void execReadData() = 0;

    // Führt den execReadData() nicht aus...
    // Wenn es im Write- Bereich aufgerufen wird, so wird der Read- Bereich überspringen...
    // (Wenn im Write- Bereich zB ein Fehler aufgetreten ist...)
    void stopCommandExecution();
    bool getStopCommandExecution();

    inline void setNoTcpExecution(bool b) { _noTcpExecution = b; }

    quint32 getResponseType() { return _responseType; }

    QString getGroupName() { return _groupName; }
    QString getActionName() { return _actionName; }

signals:
    // Signal wird geworfen, wenn die start() Methode aufgerufen wird.
    // Dieses Signal fängt der TcpMainThread ab und beginnt diesen TcpCommand
    // abzuarbeiten...
    void startCalled();
};

#endif // TCPABSTRACTCOMMAND1_H
