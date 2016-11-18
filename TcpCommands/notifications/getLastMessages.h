#ifndef TCPCOMMAND__NOTIFICATIONS_GETLASTMESSAGES_H
#define TCPCOMMAND__NOTIFICATIONS_GETLASTMESSAGES_H

#include <TcpAbstractCommand.h>
#include <QList>
#include <QHash>

class TcpCommand_notifications_getLastMessages : public TcpAbstractCommand
{
    Q_OBJECT
private:
    
    QList< QHash<QString, QVariant> > _rows;

    
public:
    explicit TcpCommand_notifications_getLastMessages(QObject *parent = 0);
    
    void execWriteData();
    void execReadData();

    inline bool hasNext();
    inline QHash<QString, QVariant> getNext();
    
public slots:
    
};

bool TcpCommand_notifications_getLastMessages::hasNext() {
    return !_rows.isEmpty();
}

QHash <QString, QVariant> TcpCommand_notifications_getLastMessages::getNext() {
    return _rows.takeFirst();
}

#endif // TCPCOMMAND__NOTIFICATIONS_GETLASTMESSAGES_H

