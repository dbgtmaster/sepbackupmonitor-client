#ifndef LOGINCONNECTIONSMANAGER_H
#define LOGINCONNECTIONSMANAGER_H

#include <QList>
#include <QPair>

class QSettings;
class QString;

namespace LoginConnectionsManager {

// Gibt eine Liste mit allen verfügbaren Verbindungen zurück.
// QPair< ID, NAME DER VERBINDUNG >
QList< QPair<QString, QString> > getAllConnectionNames();

// Gibt die Anzahl der erstellten Verbindungen zurück...
int getConnectionsCount();

// Gibt die ID der Standard- Verbindung zurück:
QString getStandardId();
void setStandardId(const QString &id);

// Gibt die nächste freie ID zurück und erhöht den internen Zähler um +1
QString getNextFreeId();

void deleteConnection(QString id);
QString addConnection(const QString& name);

void setName(const QString &id, const QString &value);
void setHostname(const QString &id, const QString &value);
void setPort(const QString &id, const QString &value);

QString getName(const QString &id);
QString getHostname(const QString &id);
QString getPort(const QString &id);
}

#endif // LOGINCONNECTIONSMANAGER_H
