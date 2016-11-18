/**
  * Modifiziert eine Kundengruppe
  */

#ifndef TCPCOMMAND__BACKUPMONITOR_CUSTOMERGROUPMOD_H
#define TCPCOMMAND__BACKUPMONITOR_CUSTOMERGROUPMOD_H

#include <TcpAbstractCommand.h>

class TcpCommand_backupMonitor_customerGroupMod : public TcpAbstractCommand
{
    Q_OBJECT
public:
    enum ACTION {
        CREATE,
        MODIFY
    };

    // Ob Gruppe nach oben oder unten geschoben werden soll.
    // _cols["position"] erwartet diesen Wert..
    enum POSITION {
        UP, DOWN
    };

private:
    
    // Write
    ACTION _action;
    quint32 _groupId;
    QHash<QString, QVariant> _cols;

    // Read:
    bool _isSuccessful;
    QString _errorMessage;
    
public:
    explicit TcpCommand_backupMonitor_customerGroupMod(QObject *parent = 0);

    void execWriteData();
    void execReadData();

    inline void setAction(ACTION action) { _action = action; }
    inline void setGroupId(quint32 groupId) { _groupId = groupId; }
    inline void setCols(QHash<QString, QVariant> cols) { _cols = cols; qDebug() << cols; }

    inline bool isSuccessful() { return _isSuccessful; }
    inline QString errorMessage() { return _errorMessage; }

    
public slots:
    
};

#endif // TCPCOMMAND__BACKUPMONITOR_CUSTOMERGROUPMOD_H

