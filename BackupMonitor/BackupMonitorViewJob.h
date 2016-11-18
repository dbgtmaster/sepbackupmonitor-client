#ifndef BACKUPMONITORVIEWJOB_H
#define BACKUPMONITORVIEWJOB_H

#include <QWidget>

namespace Ui {
class BackupMonitorViewJob;
}

class BackupMonitorViewJob : public QWidget
{
    Q_OBJECT
private:
    quint32 _jobId;
    
    int _protocolPage;
    int _protocolPrePage;
    int _protocolPostPage;

public:
    explicit BackupMonitorViewJob(quint32 jobId, QWidget *parent = 0);
    ~BackupMonitorViewJob();
    
    void reloadProtocol(int);
    void reloadProtocolPre(int);
    void reloadProtocolPost(int);

public slots:
    // Zeilenumbruch im Protokoll aktivieren / deaktivieren..
    void setProtocolWordWrap(bool);
    void setProtocolPreWordWrap(bool);
    void setProtocolPostWordWrap(bool);

    void protocolForward();
    void protocolBackward();
    void protocolSliderChanged();

    void protocolPreForward();
    void protocolPreBackward();
    void protocolPreSliderChanged();

    void protocolPostForward();
    void protocolPostBackward();
    void protocolPostSliderChanged();

private:
    Ui::BackupMonitorViewJob *ui;

    void _removeTab(const QString &tabObjectName);
};

#endif // BACKUPMONITORVIEWJOB_H
