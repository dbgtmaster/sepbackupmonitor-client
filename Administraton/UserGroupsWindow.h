#ifndef USERGROUPSWINDOW_H
#define USERGROUPSWINDOW_H

#include <QDialog>

namespace Ui {
    class UserGroupsWindow;
}

class UserGroupsWindow : public QDialog
{
    Q_OBJECT

private:

    // Wenn true, so passiert beim Anklicken einer Gruppe nichts...
    bool _disableEditGroup;

    void _reloadUserGroups();
public:
    explicit UserGroupsWindow(QWidget *parent = 0);
    ~UserGroupsWindow();

    void hideEditor();
    void showEditor();

private:
    Ui::UserGroupsWindow *ui;

public slots:
    void createNewGroupWindow();

    void startEdit();
    void ok();
    void apply();
    void remove();
};

#endif // USERGROUPSWINDOW_H
