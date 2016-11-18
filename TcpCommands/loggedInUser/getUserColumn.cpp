#include "getUserColumn.h"

TcpCommand_loggedInUser_getUserColumn::TcpCommand_loggedInUser_getUserColumn(QObject *parent) :
    TcpAbstractCommand(parent)
{
}

void TcpCommand_loggedInUser_getUserColumn::setColumn(const QByteArray &col) {
    _column = col;
}

void TcpCommand_loggedInUser_getUserColumn::execWriteData() {
    _dataStream << _column;
}

void TcpCommand_loggedInUser_getUserColumn::execReadData() {

    _dataStream >> _value;
}

QVariant& TcpCommand_loggedInUser_getUserColumn::getValue() {
    return _value;
}
