#ifndef TCPCOMMAND_MAIN_GETUSERCOLUMN_H
#define TCPCOMMAND_MAIN_GETUSERCOLUMN_H

#include <TcpAbstractCommand.h>

class TcpCommand_loggedInUser_getUserColumn : public TcpAbstractCommand
{
    Q_OBJECT
private:
    QVariant _value;
    QByteArray _column;
public:
    explicit TcpCommand_loggedInUser_getUserColumn(QObject *parent = 0);

    void setColumn(const QByteArray& col);

    void execWriteData();
    void execReadData();

    QVariant& getValue();
public slots:

};

#endif // TCPCOMMAND_MAIN_GETUSERCOLUMN_H
