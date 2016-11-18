#include "customerEditable.h"

bool TcpCommand_customers_customerEditable::_loaded = false;
bool TcpCommand_customers_customerEditable::_isEditable = false;

TcpCommand_customers_customerEditable::TcpCommand_customers_customerEditable(QObject *parent) :
    TcpAbstractCommand(parent)
{
    if (_loaded) {
        setNoTcpExecution(true);
    }
}

void TcpCommand_customers_customerEditable::execWriteData() {
    
}

void TcpCommand_customers_customerEditable::execReadData() {

    _dataStream >> _isEditable;
    _loaded = true;
}
