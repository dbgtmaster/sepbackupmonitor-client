#ifndef TCPCOMMANDSPERMISSIONS_H
#define TCPCOMMANDSPERMISSIONS_H

#include <QDialog>

#include <System/TcpCommandsPermissions/TcpCommandsItemModel.h>

#include <QList>
#include <QPair>
#include <QString>

namespace Ui {
    class TcpCommandsPermissions;
}

class TcpCommandsPermissions : public QDialog
{
    Q_OBJECT
private:
    TcpCommandsItemModel _tcpCommandsItemModel;

public:
    explicit TcpCommandsPermissions(QWidget *parent = 0);
    ~TcpCommandsPermissions();

public slots:

    // Wird aufgerufen, wenn sich eine Systemgruppe geändert hat...
    void systemGroupChanged();

    // Speichern...
    void save();

private:
    Ui::TcpCommandsPermissions *ui;
};

#endif // TCPCOMMANDSPERMISSIONS_H
