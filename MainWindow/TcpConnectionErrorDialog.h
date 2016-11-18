#ifndef TCPCONNECTIONERRORDIALOG_H
#define TCPCONNECTIONERRORDIALOG_H

#include <QDialog>

namespace Ui {
    class TcpConnectionErrorDialog;
}

class TcpConnectionErrorDialog : public QDialog
{
    Q_OBJECT

    // Wenn true, wird die Applikation am nächst Möglichen Punkt geschlossen...
    bool _closeApp;

public:
    explicit TcpConnectionErrorDialog(QWidget *parent = 0);
    ~TcpConnectionErrorDialog();

    void setErrorMessage(const QString &message);

public slots:
    void reconnect();

    void closeApp();

private:
    Ui::TcpConnectionErrorDialog *ui;
};

#endif // TCPCONNECTIONERRORDIALOG_H
