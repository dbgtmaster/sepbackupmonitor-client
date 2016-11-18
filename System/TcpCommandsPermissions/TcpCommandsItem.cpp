#include "TcpCommandsItem.h"

TcpCommandsItem::TcpCommandsItem(Type type, const QString &text) :
    _type(type), QStandardItem(text)
{

    // Nur Systemgruppe kann bearbeitet werden...
    if (type != SYSTEMGROUP) {
        setEditable(false);
    }
}

void TcpCommandsItem::setId(quint32 id) {
    setData(id);
}

quint32 TcpCommandsItem::id() {
    return data().toInt();
}
