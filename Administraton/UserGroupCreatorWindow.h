#ifndef USERGROUPCREATORWINDOW_H
#define USERGROUPCREATORWINDOW_H

#include <QDialog>

#include "Administraton/UserGroupEditor.h"

namespace Ui {
    class UserGroupCreatorWindow;
}

class UserGroupCreatorWindow : public QDialog
{
    Q_OBJECT

private:
    bool _groupCreated;

public:
    explicit UserGroupCreatorWindow(QWidget *parent = 0);
    ~UserGroupCreatorWindow();

    // Gibt true zurück, wenn eine Gruppe erstellt wurde,
    // ansonsten false...
    inline bool groupCreated() const;

public slots:
    void on_pushCreate_clicked();

private:
    Ui::UserGroupCreatorWindow *ui;
};

bool UserGroupCreatorWindow::groupCreated() const {
    return _groupCreated;
}

#endif // USERGROUPCREATORWINDOW_H
