#ifndef TCPCOMMAND__BACKUPMONITOR_GETBACKUPSTATEOFDAY_H
#define TCPCOMMAND__BACKUPMONITOR_GETBACKUPSTATEOFDAY_H

#include <TcpAbstractCommand.h>
#include <QDate>

#include <QHash>
#include <QList>

class TcpCommand_backupMonitor_getBackupStateOfDay : public TcpAbstractCommand
{
    Q_OBJECT
private:
    quint32 _customerId;
    QList<QDate> _dates;
    
    QList< QHash<QByteArray, QVariant > > _returnedList;

public:
    explicit TcpCommand_backupMonitor_getBackupStateOfDay(QObject *parent = 0);
    
    inline void setCustomerId(quint32 customerId) {
        _customerId = customerId;
    }

    inline void setDate(const QDate &d) {
        _dates.append(d);
    }

    inline void setDates(const QList<QDate> &d) {
        _dates = d;
    }

    inline QHash<QByteArray, QVariant> getNextState() {
        QHash<QByteArray, QVariant> tmp = _returnedList.front();
        _returnedList.pop_front();
        return tmp;
    }

    bool hasNextState() {
        return !(_returnedList.isEmpty());
    }

    void execWriteData();
    void execReadData();
    
public slots:
    
};

#endif // TCPCOMMAND__BACKUPMONITOR_GETBACKUPSTATEOFDAY_H

