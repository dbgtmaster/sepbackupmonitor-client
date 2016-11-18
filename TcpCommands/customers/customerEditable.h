#ifndef TCPCOMMAND__CUSTOMERS_CUSTOMEREDITABLE_H
#define TCPCOMMAND__CUSTOMERS_CUSTOMEREDITABLE_H

#include <TcpAbstractCommand.h>

class TcpCommand_customers_customerEditable : public TcpAbstractCommand
{
    Q_OBJECT
private:
    
    static bool _isEditable;
    static bool _loaded;        // Ob der Wert bereits geladen wurde?
    
public:
    explicit TcpCommand_customers_customerEditable(QObject *parent = 0);
    
    void execWriteData();
    void execReadData();

    inline bool isEditable() { return _isEditable; }
    
public slots:
    
};

#endif // TCPCOMMAND__CUSTOMERS_CUSTOMEREDITABLE_H

