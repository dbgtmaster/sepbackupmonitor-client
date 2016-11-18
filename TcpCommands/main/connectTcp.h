#ifndef TCPCOMMAND_CONNECTTCP_H
#define TCPCOMMAND_CONNECTTCP_H

#include <TcpAbstractCommand.h>

#include <QObject>
#include <QByteArray>

class TcpCommand_main_connectTcp : public TcpAbstractCommand
{
    Q_OBJECT

private:
    QString _hostname;
    quint16 _port;

    static QString _lastHostname;
    static quint16 _lastPort;

    // Fehler aufgetreten?
    bool _isError;
    QString _errorString;

    void setError(const QString&);

public:

    TcpCommand_main_connectTcp(QObject *parent = 0);

    void setHostname(const QString hostname);
    void setPort(const quint16 port);

    void useLastHostData();

    void execWriteData();
    void execReadData();

    bool isError();
    QString& errorString();

private slots:
    void tcpError(QAbstractSocket::SocketError,QString,QString);

signals:

    // Signal wird geworfen, wenn Verbindung zum Tcp- Server erfolgreich aufgebaut wurde.
    void successful();

    // Signal wird geworfen, wenn ein Fehler aufgetreten ist...
    void error();
};

#endif // TCPCOMMAND_CONNECTTCP_H
