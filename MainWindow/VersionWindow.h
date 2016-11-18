#ifndef VERSIONWINDOW_H
#define VERSIONWINDOW_H

#include <QWidget>

namespace Ui {
    class VersionWindow;
}

class VersionWindow : public QWidget
{
    Q_OBJECT

public:
    explicit VersionWindow(QWidget *parent = 0);
    ~VersionWindow();

public slots:
    void getServerVersion();

private:
    Ui::VersionWindow *ui;
};

#endif // VERSIONWINDOW_H
