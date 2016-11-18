/**
  * LÃ¶scht eine Benutzergruppe
  */

#ifndef TCPCOMMAND_ADMINISTRATION_USERGROUPDELETE_H
#define TCPCOMMAND_ADMINISTRATION_USERGROUPDELETE_H

#include <TcpAbstractCommand.h>

#include <QObject>
#include <QString>

class TcpCommand_administration_userGroupDelete : public TcpAbstractCommand
{
    Q_OBJECT

private:
    quint32 _id;

    bool _successful;

public:

    TcpCommand_administration_userGroupDelete(QObject *parent = 0);

    inline void setId(quint32 id);

    void execWriteData();
    void execReadData();

   inline bool successful();
};

void TcpCommand_administration_userGroupDelete::setId(quint32 id) {
    _id = id;
}

bool TcpCommand_administration_userGroupDelete::successful() {
    return _successful;
}

#endif // TCPCOMMAND_ADMINISTRATION_USERGROUPDELETE_H


