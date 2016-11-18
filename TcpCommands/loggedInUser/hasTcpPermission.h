#ifndef TCPCOMMAND__LOGGEDINUSER_HASTCPPERMISSION_H
#define TCPCOMMAND__LOGGEDINUSER_HASTCPPERMISSION_H

#include <TcpAbstractCommand.h>

class TcpCommand_loggedInUser_hasTcpPermission : public TcpAbstractCommand
{
    Q_OBJECT
private:
    
    QString _group;
    QString _action;

    bool _hasPermission;
    
public:
    explicit TcpCommand_loggedInUser_hasTcpPermission(QObject *parent = 0);
    
    inline void setData(const QString &group, const QString &action) {
        _group = group;
        _action = action;
    }

    inline bool hasPermission() {
        return _hasPermission;
    }

    void execWriteData();
    void execReadData();
    
public slots:
    
};

namespace TcpCommands {
    // Gibt true zurück, wenn eingeloggter Benutzer bestimmte Berechtigung hat, ansonsten false...
    bool hasTcpPermission(const QString &group, const QString &action);
}

#endif // TCPCOMMAND__LOGGEDINUSER_HASTCPPERMISSION_H

