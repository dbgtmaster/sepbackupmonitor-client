#ifndef TCPCOMMAND__NOTIFICATIONS_CREATE_H
#define TCPCOMMAND__NOTIFICATIONS_CREATE_H

#include <TcpAbstractCommand.h>

class TcpCommand_notifications_create : public TcpAbstractCommand
{
    Q_OBJECT
private:
    
    QByteArray _data;
    quint8 _priority;
    QByteArray _type;

public:
    explicit TcpCommand_notifications_create(QObject *parent = 0);
    
    // Daten, welche mittels eines QDataStream geschrieben wurden:
    inline void setData(QByteArray &data);

    // 0 -> normale Nachricht, 1 -> wird hervorgehoben, 2 -> popup erscheint, wenn Benutzern eingeloggt ist.
    inline void setPriority(quint8 priority);

    // Type, wie zB. "backup", "watchbox",..
    inline void setType(QByteArray &type);

    void execWriteData();
    void execReadData();
    
public slots:
    
};

void TcpCommand_notifications_create::setData(QByteArray &data) {
    _data = data;
}

void TcpCommand_notifications_create::setPriority(quint8 priority) {
    _priority = priority;
}

void TcpCommand_notifications_create::setType(QByteArray &type) {
    _type = type;
}

#endif // TCPCOMMAND__NOTIFICATIONS_CREATE_H

