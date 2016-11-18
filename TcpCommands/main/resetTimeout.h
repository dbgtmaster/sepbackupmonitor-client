/**
  * Resetet am intranet-server den Timeout Zähler.
  * Wenn die Verbindung zum intranet-server für 5 Minuten keine Pakete übertragen werden, wird diese automatisch beendet,
  * um am Server keine Leichen- Verbindungen zu haben, welche für immer laufen.
  * Desweiteren soll damit verhindert werden, dass die Verbindung Aufgrund eines Betriebssystem Timeouts oder ähnliches abbricht.
  *
  * Am Intranet-server wurde eine solche methode nicht definiert, da diese direkt am Server vom TcpSocketThread abgearbeitet wird (um eine
  * Antwort so schnell wie möglich zu erhalten).
  */

#ifndef TCPCOMMAND__MAIN_RESETTIMEOUT_H
#define TCPCOMMAND__MAIN_RESETTIMEOUT_H

#include <TcpAbstractCommand.h>

class TcpCommand_main_resetTimeout : public TcpAbstractCommand
{
    Q_OBJECT
private:
    
    
public:
    explicit TcpCommand_main_resetTimeout(QObject *parent = 0);
    
    void execWriteData();
    void execReadData();
    
public slots:
    
};

#endif // TCPCOMMAND__MAIN_RESETTIMEOUT_H

