#include "Utils/UtilDateTime.h"

#include <QDateTime>
#include <QString>
#include <QStringList>

#include <math.h>
#include <QDate>
#include <QDebug>

QString UtilDateTime::getSinceTime(const QDateTime &d, bool printSince) {

    // Wie viele Sekunden sind zwischen der übergebenen & aktuellen Zeit vergangen:
    uint seconds = QDateTime::currentDateTime().toTime_t() - d.toTime_t();

    QString timeString;
    if (printSince) {
        timeString = "vor %1 %2";
    }
    else {
        timeString = "%1 %2";
    }

    if (seconds <= 60) {
        return timeString.arg( QString::number(seconds), "Sekunden");
    }
    else if (seconds <= 3600) {
        return timeString.arg( QString::number( floor( static_cast<float>( seconds / 60))) , "Minuten");
    }
    else if (seconds <= 86400) {
        return timeString.arg( QString::number(floor( (seconds / 3600.0) * 10) / 10) , "Stunden");
    }
    else {
        return timeString.arg( QString::number(floor( (seconds / 86400.0) * 100) / 100) , "Tage");
    }
}

QString UtilDateTime::getDayOfWeek(const QDate &d) {

    if (!d.isValid()) {
        return "UtilDateTime::getDayOfWeek(): invalid date given!";
    }

    QStringList list;
    list << "Montag" << "Dienstag" << "Mittwoch" << "Donnerstag" << "Freitag" << "Samstag" << "Sonntag";

    return list.at(d.dayOfWeek() - 1);
}

QString UtilDateTime::getShortDayOfWeek(const QDate &d) {

    if (!d.isValid()) {
        return "UtilDateTime::getDayOfWeek(): invalid date given!";
    }

    QStringList list;
    list << "MO" << "DI" << "MI" << "DO" << "FR" << "SA" << "SO";

    return list.at(d.dayOfWeek() - 1);
}
