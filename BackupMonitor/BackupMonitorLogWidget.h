#ifndef BACKUPMONITORLOGWIDGET_H
#define BACKUPMONITORLOGWIDGET_H

#include <QWidget>

namespace Ui {
class BackupMonitorLogWidget;
}

class BackupMonitorLogWidget : public QWidget
{
    Q_OBJECT

private:
    quint32 _customerID;

    QString _logsHtml;
public:
    explicit BackupMonitorLogWidget(QWidget *parent = 0);
    ~BackupMonitorLogWidget();

    void setProperties(quint32 customerID, const QString &customerName, const QString &taskName,
                       bool showLogOnlyForThisTask = true);

    void show();
public slots:
    void createLog();
    void initLogEntries();

private:
    Ui::BackupMonitorLogWidget *ui;
};

#endif // BACKUPMONITORLOGWIDGET_H
