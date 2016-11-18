#ifndef TCPCOMMAND__BACKUPMONITOR_CUSTOMERGROUPDELETE_H
#define TCPCOMMAND__BACKUPMONITOR_CUSTOMERGROUPDELETE_H

#include <TcpAbstractCommand.h>

class TcpCommand_backupMonitor_customerGroupDelete : public TcpAbstractCommand
{
    Q_OBJECT
private:
    
    quint32 _id;
    
public:
    explicit TcpCommand_backupMonitor_customerGroupDelete(QObject *parent = 0);
    
    void execWriteData();
    void execReadData();
    
    inline void setGroupId(quint32 id) { _id = id; }

public slots:
    
};

#endif // TCPCOMMAND__BACKUPMONITOR_CUSTOMERGROUPDELETE_H

