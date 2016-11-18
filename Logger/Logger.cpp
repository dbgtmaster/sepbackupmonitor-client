#include "Logger.h"

#include <QDateTime>
#include <QDebug>
#include <QCoreApplication>

Logger* Logger::_logger = 0;

// Konstruktor, ist private...
Logger::Logger(QObject *parent) : QObject(parent) {

}

Logger::~Logger() {

    if (_logger != 0) {
        delete _logger;
    }
}

// Erstellt das Singleton Objekt:
Logger* Logger::instance() {

    if (_logger == 0) {
        _logger = new Logger();
    }

    return _logger;
}

// Fügt einen Appender hinzu...
void Logger::addAppender(LoggerAbstractAppender *app) {
    _appenders.append(app);
}

void Logger::writeAppenders(const QString &msg) {

    for (int i = 0; i < _appenders.count(); i++) {
        _appenders.at(i)->write(msg);
    }
}

QString Logger::prepareMsg(const QString &logLevel, const QString &msg) {

    return QString("%1 [%2] [%3]: %4")
            .arg(QDateTime::currentDateTime().toString(),
                 logLevel,
                 QString::number(QCoreApplication::applicationPid()),
                 msg
                );
}

void Logger::debug(const QString &msg) {

    writeAppenders( prepareMsg("DEBUG", msg) );
}

void Logger::notice(const QString &msg) {

    writeAppenders( prepareMsg("NOTICE", msg) );
}

void Logger::warning(const QString &msg) {

    writeAppenders( prepareMsg("WARNING", msg) );
}

void Logger::error(const QString &msg) {

    writeAppenders( prepareMsg("ERROR", msg) );
}

void Logger::fatal(const QString &msg) {

    writeAppenders( prepareMsg("FATAL", msg) );
}
