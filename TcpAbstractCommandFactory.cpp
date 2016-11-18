#include "TcpAbstractCommandFactory.h"

#include <TcpAbstractCommand.h>

// Konstruktor:
TcpAbstractCommandFactory::TcpAbstractCommandFactory(const QByteArray &groupName, const QByteArray &actionName)
    : _groupName(groupName), _actionName(actionName) {

}

TcpAbstractCommand* TcpAbstractCommandFactory::getCommand() {

    TcpAbstractCommand* command = createCommand();

    command->setGroupActionName(_groupName, _actionName);

    return command;
}

// Destroy something...
TcpAbstractCommandFactory::~TcpAbstractCommandFactory() {

}
