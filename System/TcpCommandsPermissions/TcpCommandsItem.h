#ifndef TCPCOMMANDSITEM_H
#define TCPCOMMANDSITEM_H

#include <QStandardItem>

class TcpCommandsItem : public QStandardItem
{
public:
    enum Type { COMMAND = 1001, COMMANDGROUP, SYSTEMGROUP, EMPTY };

private:
    Type _type;

public:

    explicit TcpCommandsItem(Type type, const QString &text);

    // Setzt die ID des Befehls, bzw. der Gruppe, je nachdem, was dieses Item verwaltet...
    void setId(quint32 id);
    quint32 id();

signals:

public slots:

};

#endif // TCPCOMMANDSITEM_H
