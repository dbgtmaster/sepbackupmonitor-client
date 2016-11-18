#ifndef SERVERPERFORMANCESTATISTICS_H
#define SERVERPERFORMANCESTATISTICS_H

#include <QDialog>

namespace Ui {
class ServerPerformanceStatistics;
}

class ServerPerformanceStatistics : public QDialog
{
    Q_OBJECT
    
public:
    explicit ServerPerformanceStatistics(QWidget *parent = 0);
    ~ServerPerformanceStatistics();
    
private:
    Ui::ServerPerformanceStatistics *ui;

public slots:
    void reloadStats();
};

#endif // SERVERPERFORMANCESTATISTICS_H
