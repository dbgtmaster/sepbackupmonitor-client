/**
  * Liefert zu einem Job das Protokoll zurÃŒck...
  */

#ifndef TCPCOMMAND_BACKUPMONITOR_GETJOBPROTOCOL_H
#define TCPCOMMAND_BACKUPMONITOR_GETJOBPROTOCOL_H

#include <TcpAbstractCommand.h>

#include <QObject>
#include <QString>

class TcpCommand_backupMonitor_getJobProtocol : public TcpAbstractCommand
{
    Q_OBJECT
    
public:

    enum PROTOCOL_TYPE {
        MAIN,
        PRE,
        POST
    };

private:

    quint32 _jobId;
    PROTOCOL_TYPE _protocolType;
    quint32 _protocolPage;

    QByteArray _protocol;
    quint32 _maxPages;
public:
    
    TcpCommand_backupMonitor_getJobProtocol(QObject *parent = 0);
    
    void execWriteData();
    void execReadData();

    inline void setJobId(quint32 id) {
        _jobId = id;
    }
    inline void setProtocolType(PROTOCOL_TYPE t) {
        _protocolType = t;
    }
    inline void setProtocolPage(quint32 page) {
        _protocolPage = page;
    }
    
    inline QByteArray getProtocol() {
        qDebug() << _protocol;
        return qUncompress(_protocol);
    }

    // Liefert die Anzahl der verfügbaren Protokollseiten.
    quint32 getMaxPages() {
        return _maxPages;
    }
};

#endif // TCPCOMMAND_BACKUPMONITOR_GETJOBPROTOCOL_H


