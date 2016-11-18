#ifndef TCPCOMMAND_MAIN_USERLOGIN_H
#define TCPCOMMAND_MAIN_USERLOGIN_H

#include <TcpAbstractCommand.h>

#include <QObject>
#include <QString>

class TcpCommand_main_userLogin : public TcpAbstractCommand
{
    Q_OBJECT

private:
    // Daten, mit welche eingeloggt werden soll:
    QString _username;
    QString _password;

    static QString _lastUsername;
    static QString _lastPassword;

    // Ob Login erfolgreich oder nicht...
    bool _success;
public:
    TcpCommand_main_userLogin(QObject *parent = 0);

    void setUsername(QString username);
    void setPassword(QString password);

    // Setzt die zuletzt verwendeten Login- Daten:
    void useSavedLoginData();

    // Ob Login erfolgreich war oder nicht...
    bool isSuccess();

    void execWriteData();
    void execReadData();

signals:

};

#endif // TCPCOMMAND_MAIN_USERLOGIN_H
