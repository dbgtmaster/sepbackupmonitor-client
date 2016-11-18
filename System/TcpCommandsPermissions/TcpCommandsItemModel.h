#ifndef TCPCOMMANDSITEMMODEL_H
#define TCPCOMMANDSITEMMODEL_H

#include <QStandardItemModel>

#include <QHash>

class TcpCommandsItem;

class TcpCommandsItemModel : public QStandardItemModel
{
    Q_OBJECT

private:

    // Im Cache werden alle bereits gespeicherten TcpCommand-Gruppen + deren Item gespeichert,
    // damit die Befehle in die richtige Gruppe eingefügt werden kann...
    QMap<QString, TcpCommandsItem*> _cacheCommandGroups;

    // Cache mit allen Systemgruppen:
    QMap<quint32, QString> _cacheSystemGroups;

    // Alle mitprotokollierten Änderungen:
    // [commandId], [New SysGroup ID]
    QHash<quint32, quint32> _changes;

    QString getSystemGroupNameById(quint32 id);
public:

    struct tcpCommand {
        tcpCommand(const quint32 id_, const QString &group_, const QString &action_, const quint32 systemGroupId_) :
            id(id_), group(group_), action(action_), systemGroupId(systemGroupId_) { }

        quint32 id;
        QString group;
        QString action;
        quint32 systemGroupId;
    };

    explicit TcpCommandsItemModel(QObject *parent = 0);

    // Fügt einen neuen Befehl ein:
    void addCommand(const tcpCommand &command);

    // Fügt eine Systemgruppe hinzu:
    void addSystemGroup(quint32 id, const QString &name);
    QMap<quint32, QString>& getAllSystemGroups();

    // Wird überladen, um alle Änderungen mitzu- protokollieren...
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    // Gibt eine Liste mit allen Änderungen zurück:
    // @return QList< [CommandId], [New sysgroup id] >
    QHash<quint32, quint32>& getAllChangedSysGroups();

public slots:

};

#endif // TCPCOMMANDSITEMMODEL_H
