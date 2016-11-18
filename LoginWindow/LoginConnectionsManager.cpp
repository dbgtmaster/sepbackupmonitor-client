#include "LoginConnectionsManager.h"

#include <QSettings>
#include <QList>
#include <QStringList>
#include <QPair>
#include <QDebug>

QList< QPair<QString, QString> > LoginConnectionsManager::getAllConnectionNames() {

    QSettings settings;
    settings.beginGroup("connections");
    QStringList ids = settings.childGroups();

    // Dazugehörige Namen der Gruppen ermitteln:
    QList< QPair<QString, QString> > list;
    QString id;
    foreach (id, ids) {

        list.append( QPair<QString, QString>(id, settings.value( QString("%1/name").arg(id) ).toString() ) );
    }

    return list;
}

int LoginConnectionsManager::getConnectionsCount() {

    QSettings settings;
    settings.beginGroup("connections");
    qDebug() << settings.childGroups();
    return settings.childGroups().size();
}

QString LoginConnectionsManager::getStandardId() {

    QSettings settings;
    return settings.value("connections/standardId", "-1").toString();
}

void LoginConnectionsManager::setStandardId(const QString &id) {

    QSettings settings;
    settings.setValue("connections/standardId", id.toInt());
}

void LoginConnectionsManager::deleteConnection(QString id) {

    QSettings settings;
    settings.remove( QString("connections/%1").arg(id) );
}

QString LoginConnectionsManager::addConnection(const QString &name) {

    if (name.size() < 1) return 0;

    QSettings settings;

    // Die nächste freie ID ermitteln:
    QString nextId = settings.value("connections/nextId").toString();
    if (nextId.size() == 0) nextId = "1";
    settings.setValue("connections/nextId", QVariant( nextId.toInt() + 1 ) );

    settings.setValue( QString("connections/%1/name").arg(nextId), name );

    return nextId;
}

void LoginConnectionsManager::setHostname(const QString &id, const QString &value) {
    QSettings settings;
    settings.setValue( QString("connections/%1/hostname").arg(id), value );
}

void LoginConnectionsManager::setName(const QString &id, const QString &value) {
    QSettings settings;
    settings.setValue( QString("connections/%1/name").arg(id), value );
}

void LoginConnectionsManager::setPort(const QString &id, const QString &value) {
    QSettings settings;
    settings.setValue( QString("connections/%1/port").arg(id), value.toInt() );
}

QString LoginConnectionsManager::getPort(const QString &id) {
    QSettings settings;
    return settings.value( QString("connections/%1/port").arg(id) ).toString();
}

QString LoginConnectionsManager::getName(const QString &id) {
    QSettings settings;
    return settings.value( QString("connections/%1/name").arg(id) ).toString();
}

QString LoginConnectionsManager::getHostname(const QString &id) {
    QSettings settings;
    return settings.value( QString("connections/%1/hostname").arg(id) ).toString();
}
