#ifndef USERGROUPEDITOR_H
#define USERGROUPEDITOR_H

#include <QWidget>
#include <QList>
#include <QHash>

#include "ui_UserGroupEditor.h"

namespace TcpCommandsHelper {
    struct ldapFilter;
}

namespace Ui {
    class UserGroupEditor;
}

class QCheckBox;

class UserGroupEditor : public QWidget
{
    Q_OBJECT

private:
    // Eine Liste, welche Checkbox zu welcher Systemgruppe gehört.
    QHash<QCheckBox*, quint32> _sysGroupsCheckbox;

public:
    explicit UserGroupEditor(QWidget *parent = 0);
    ~UserGroupEditor();

    /**
      * Daten setzen:
      */
    inline void setGroupName(const QString &name);
    inline void setIsLocked(const bool t);

    void setLdapFilter1(const TcpCommandsHelper::ldapFilter &f);
    void setLdapFilter2(const TcpCommandsHelper::ldapFilter &f);
    void setLdapFilter3(const TcpCommandsHelper::ldapFilter &f);
    inline void setIsLdapFilter1Enabled(const bool t);
    inline void setIsLdapFilter2Enabled(const bool t);
    inline void setIsLdapFilter3Enabled(const bool t);

    void setChoosedSystemGroups(const QList<quint32> &l);

    /**
      * Daten ermitteln:
      */
    // Daten über den LDAP- Filter ermitteln:
    TcpCommandsHelper::ldapFilter getLdapFilter1();
    TcpCommandsHelper::ldapFilter getLdapFilter2();
    TcpCommandsHelper::ldapFilter getLdapFilter3();
    inline bool isLdapFilter1Enabled();
    inline bool isLdapFilter2Enabled();
    inline bool isLdapFilter3Enabled();

    // Allgemeine Daten ermitteln:
    inline const QString getGroupName() const;
    inline bool isLocked() const;

    // Gibt eine Liste mit allen ausgewählten Systemgruppen zurück...
    QList<quint32> getChoosedSystemGroups();

    // Versteckt den Button, um die aktuelle Gruppe zu löschen:
    void hideGroupDeleteButton();

private:
    Ui::UserGroupEditor *ui;

signals:

    void groupDeleteClicked();

    // Signal wird geworfen, wenn ein Wert im Formular geändert wird.
    void valueChanged(bool t = true);

public slots:

    // Überprüft die LDAP- Zuordnung...
    void testLDAPMembershipSettings();
};

void UserGroupEditor::setGroupName(const QString &name) {
    ui->inputName->setText(name);
}

void UserGroupEditor::setIsLocked(bool t) {
    ui->checkboxLocked->setChecked(t);
}

void UserGroupEditor::setIsLdapFilter1Enabled(const bool t) {
    ui->groupBoxLdapFilter1->setChecked(t);
}

void UserGroupEditor::setIsLdapFilter2Enabled(const bool t) {
    ui->groupBoxLdapFilter2->setChecked(t);
}

void UserGroupEditor::setIsLdapFilter3Enabled(const bool t) {
    ui->groupBoxLdapFilter3->setChecked(t);
}

const QString UserGroupEditor::getGroupName() const {
    return ui->inputName->text();
}

bool UserGroupEditor::isLocked() const {
    return ui->checkboxLocked->isChecked();
}

bool UserGroupEditor::isLdapFilter1Enabled() {
    return ui->groupBoxLdapFilter1->isChecked();
}

bool UserGroupEditor::isLdapFilter2Enabled() {
    return ui->groupBoxLdapFilter2->isChecked();
}

bool UserGroupEditor::isLdapFilter3Enabled() {
    return ui->groupBoxLdapFilter3->isChecked();
}

#endif // USERGROUPEDITOR_H
