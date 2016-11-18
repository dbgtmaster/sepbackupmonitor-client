#include "userLogin.h"

#include <QDebug>

QString TcpCommand_main_userLogin::_lastPassword;
QString TcpCommand_main_userLogin::_lastUsername;

TcpCommand_main_userLogin::TcpCommand_main_userLogin(QObject *parent) : TcpAbstractCommand(parent)
{
}

void TcpCommand_main_userLogin::setUsername(QString username) {
    _username = username;
    _lastUsername = username;
}

void TcpCommand_main_userLogin::setPassword(QString password) {
    _password = password;
    _lastPassword = password;
}

void TcpCommand_main_userLogin::useSavedLoginData() {
    _username = _lastUsername;
    _password = _lastPassword;
}

bool TcpCommand_main_userLogin::isSuccess() {
    return _success;
}

void TcpCommand_main_userLogin::execWriteData() {

    TcpCommandsHelper::userLogin loginData;
    loginData.username = _username;
    loginData.password = _password;

    // Logindaten in den Stream schreiben:
    _dataStream << loginData;

}

void TcpCommand_main_userLogin::execReadData() {

    // Antwort auswerten:
    TcpCommandsHelper::userLoginResponse loginDataResponse;
    _dataStream >> loginDataResponse;

    _success = loginDataResponse.success;
}
