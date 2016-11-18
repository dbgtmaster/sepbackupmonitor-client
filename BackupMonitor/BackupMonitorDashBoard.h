#ifndef BACKUPMONITORDASHBOARD_H
#define BACKUPMONITORDASHBOARD_H

#include <QWidget>

namespace Ui {
class BackupMonitorDashBoard;
}

class BackupMonitorDashBoard : public QWidget
{
    Q_OBJECT

public:
    explicit BackupMonitorDashBoard(QWidget *parent = 0);
    ~BackupMonitorDashBoard();

private:
    Ui::BackupMonitorDashBoard *ui;

private slots:
    void _refreshDashBoard();
};

#endif // BACKUPMONITORDASHBOARD_H
