#ifndef BACKUPMONITORCLIENTSTATUSPLEASEWAITWIDGET_H
#define BACKUPMONITORCLIENTSTATUSPLEASEWAITWIDGET_H

#include <QWidget>

namespace Ui {
class BackupMonitorClientStatusPleaseWaitWidget;
}

class BackupMonitorClientStatusPleaseWaitWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit BackupMonitorClientStatusPleaseWaitWidget(QWidget *parent = 0);
    ~BackupMonitorClientStatusPleaseWaitWidget();

public slots:
    void reloadState(QString, int, int);

private:
    Ui::BackupMonitorClientStatusPleaseWaitWidget *ui;
};

#endif // BACKUPMONITORCLIENTSTATUSPLEASEWAITWIDGET_H
