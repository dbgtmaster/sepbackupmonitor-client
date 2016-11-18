#ifndef CUSTOMERVIEWWINDOW_H
#define CUSTOMERVIEWWINDOW_H

#include <QWidget>

class CustomerViewModule;

namespace Ui {
class CustomerViewWindow;
}

class CustomerViewWindow : public QWidget
{
    Q_OBJECT

private:
    int _customerId;

    bool _customerDataEditable;

public:
    explicit CustomerViewWindow(int customerId, QWidget *parent = 0);
    ~CustomerViewWindow();


public slots:
    void saveSettings();
    void removeCustomer();

private:
    Ui::CustomerViewWindow *ui;
};

#endif // CUSTOMERVIEWWINDOW_H
