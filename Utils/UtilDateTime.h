#ifndef UTILDATETIME_H
#define UTILDATETIME_H

class QString;
class QDateTime;
class QDate;

namespace UtilDateTime {

    // Schreibt ein Datum / Uhrzeit in der "vor 10 Minuten", "vor 2 Stunden", "vor 2 Tagen"..
    // QDateTime d: Uhrzeit / Datum
    // bool printSince: Soll immer das Wort "vor" vorgesetzt werden?? zB. "vor 10 Minuten"
    QString getSinceTime(const QDateTime &d, bool printSince = true);

    // Gibt den Namen des Wochentags zurück (Montag, Dienstag,..)
    QString getDayOfWeek(const QDate &d);

    // Gibt den Namen des Wochentags in Kurzform zurück (MO, DI,...)
    QString getShortDayOfWeek(const QDate &d);
}

#endif // UTILDATETIME_H
