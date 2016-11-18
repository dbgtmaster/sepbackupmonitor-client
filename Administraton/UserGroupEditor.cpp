#include "UserGroupEditor.h"
#include "ui_UserGroupEditor.h"

#include "TcpCommands/system/getAllSystemGroups.h"
#include "TcpCommands/administration/testLDAPGroupMemberships.h"

#include "EventFilters/LabelToCheckboxEventFilter.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QCheckBox>

UserGroupEditor::UserGroupEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserGroupEditor)
{
    ui->setupUi(this);

    // Alle Systemgruppen holen...
    CREATE_TCP_COMMAND(sysGroups, system, getAllSystemGroups);
    sysGroups->setGetDescription(true);
    sysGroups->startWait();

    /**
      * Alle Systemgruppen zur Auswahl ins Widget einfügen:
      */
    QGridLayout *sysGroupsLayout = new QGridLayout( ui->widgetChooseSystemGroups );
    int row = 0;
    while (sysGroups->hasNext()) {

        QCheckBox *checkboxActivated = new QCheckBox(ui->widgetChooseSystemGroups);
        _sysGroupsCheckbox.insert(checkboxActivated, sysGroups->groupId());

        LabelToCheckboxEventFilter *labelEventFilter = new LabelToCheckboxEventFilter(checkboxActivated, this);

        QLabel *name = new QLabel("<b>" + sysGroups->groupName() + "</b>", ui->widgetChooseSystemGroups);
        name->setWordWrap(true);
        name->installEventFilter(labelEventFilter);

        QLabel *description = new QLabel(sysGroups->description(), ui->widgetChooseSystemGroups);
        description->setWordWrap(true);
        QPalette palette = description->palette();
        palette.setColor(description->foregroundRole(), Qt::gray);
        description->setPalette(palette);
        description->setStyleSheet("QLabel { margin-bottom: 14px }");
        description->installEventFilter(labelEventFilter);

        sysGroupsLayout->addWidget(checkboxActivated, row, 0);
        sysGroupsLayout->addWidget(name, row, 1, Qt::AlignLeft);
        sysGroupsLayout->addWidget(description, ++row, 0, 1, 0);

        connect(checkboxActivated, SIGNAL(stateChanged(int)), this, SIGNAL(valueChanged()));

        row++;
    }

    sysGroupsLayout->setRowStretch(row, 2);
    sysGroupsLayout->setColumnStretch(2, 2);

    /**
      * Combo- Boxen ONE / SUB Auswahl für LDAP- Suche:
      */
    ui->comboBoxLdapFilter1Scope->addItem("BASE", TcpCommandsHelper::LDAP_BASE);
    ui->comboBoxLdapFilter1Scope->addItem("ONE", TcpCommandsHelper::LDAP_ONE);
    ui->comboBoxLdapFilter1Scope->addItem("SUB", TcpCommandsHelper::LDAP_SUB);
    ui->comboBoxLdapFilter2Scope->addItem("BASE", TcpCommandsHelper::LDAP_BASE);
    ui->comboBoxLdapFilter2Scope->addItem("ONE", TcpCommandsHelper::LDAP_ONE);
    ui->comboBoxLdapFilter2Scope->addItem("SUB", TcpCommandsHelper::LDAP_SUB);
    ui->comboBoxLdapFilter3Scope->addItem("BASE", TcpCommandsHelper::LDAP_BASE);
    ui->comboBoxLdapFilter3Scope->addItem("ONE", TcpCommandsHelper::LDAP_ONE);
    ui->comboBoxLdapFilter3Scope->addItem("SUB", TcpCommandsHelper::LDAP_SUB);

    ui->tabWidget->setCurrentIndex(0);

    connect(ui->pushTestLDAPMembershipSettings, SIGNAL(clicked()),
            this, SLOT(testLDAPMembershipSettings()));

    connect(ui->pushDeleteGroup, SIGNAL(clicked()), this, SIGNAL(groupDeleteClicked()));

    connect(ui->inputLdapFilter1Dn, SIGNAL(textChanged(QString)), this, SIGNAL(valueChanged()));
    connect(ui->inputLdapFilter2Dn, SIGNAL(textChanged(QString)), this, SIGNAL(valueChanged()));
    connect(ui->inputLdapFilter3Dn, SIGNAL(textChanged(QString)), this, SIGNAL(valueChanged()));
    connect(ui->inputLdapFilter1SearchFilter, SIGNAL(textChanged(QString)), this, SIGNAL(valueChanged()));
    connect(ui->inputLdapFilter2SearchFilter, SIGNAL(textChanged(QString)), this, SIGNAL(valueChanged()));
    connect(ui->inputLdapFilter3SearchFilter, SIGNAL(textChanged(QString)), this, SIGNAL(valueChanged()));
    connect(ui->inputName, SIGNAL(textChanged(QString)), this, SIGNAL(valueChanged()));
    connect(ui->checkboxLocked, SIGNAL(clicked()), this, SIGNAL(valueChanged()));
    connect(ui->comboBoxLdapFilter1Scope, SIGNAL(activated(int)), this, SIGNAL(valueChanged()));
    connect(ui->comboBoxLdapFilter2Scope, SIGNAL(activated(int)), this, SIGNAL(valueChanged()));
    connect(ui->comboBoxLdapFilter3Scope, SIGNAL(activated(int)), this, SIGNAL(valueChanged()));
    connect(ui->groupBoxLdapFilter1, SIGNAL(clicked()), this, SIGNAL(valueChanged()));
    connect(ui->groupBoxLdapFilter2, SIGNAL(clicked()), this, SIGNAL(valueChanged()));
    connect(ui->groupBoxLdapFilter3, SIGNAL(clicked()), this, SIGNAL(valueChanged()));
}

UserGroupEditor::~UserGroupEditor()
{
    delete ui;
}

void UserGroupEditor::setLdapFilter1(const TcpCommandsHelper::ldapFilter &f) {

    ui->inputLdapFilter1Dn->setText(f.baseDn);
    ui->comboBoxLdapFilter1Scope->setCurrentIndex(
                ui->comboBoxLdapFilter1Scope->findData(f.scope)
                );
    ui->inputLdapFilter1SearchFilter->setText(f.filter);
}

void UserGroupEditor::setLdapFilter2(const TcpCommandsHelper::ldapFilter &f) {

    ui->inputLdapFilter2Dn->setText(f.baseDn);
    ui->comboBoxLdapFilter2Scope->setCurrentIndex(
                ui->comboBoxLdapFilter2Scope->findData(f.scope)
                );
    ui->inputLdapFilter2SearchFilter->setText(f.filter);
}

void UserGroupEditor::setLdapFilter3(const TcpCommandsHelper::ldapFilter &f) {

    ui->inputLdapFilter3Dn->setText(f.baseDn);
    ui->comboBoxLdapFilter3Scope->setCurrentIndex(
                ui->comboBoxLdapFilter3Scope->findData(f.scope)
                );
    ui->inputLdapFilter3SearchFilter->setText(f.filter);
}

TcpCommandsHelper::ldapFilter UserGroupEditor::getLdapFilter1() {
    TcpCommandsHelper::ldapFilter filter;
    filter.baseDn = ui->inputLdapFilter1Dn->text();
    filter.scope = static_cast<TcpCommandsHelper::ldapScope>( ui->comboBoxLdapFilter1Scope->itemData( ui->comboBoxLdapFilter1Scope->currentIndex() ).toInt() );
    filter.filter = ui->inputLdapFilter1SearchFilter->text();

    return filter;
}

TcpCommandsHelper::ldapFilter UserGroupEditor::getLdapFilter2() {
    TcpCommandsHelper::ldapFilter filter;
    filter.baseDn = ui->inputLdapFilter2Dn->text();
    filter.scope = static_cast<TcpCommandsHelper::ldapScope>( ui->comboBoxLdapFilter2Scope->itemData( ui->comboBoxLdapFilter2Scope->currentIndex() ).toInt() );
    filter.filter = ui->inputLdapFilter2SearchFilter->text();

    return filter;
}

TcpCommandsHelper::ldapFilter UserGroupEditor::getLdapFilter3() {
    TcpCommandsHelper::ldapFilter filter;
    filter.baseDn = ui->inputLdapFilter3Dn->text();
    filter.scope = static_cast<TcpCommandsHelper::ldapScope>( ui->comboBoxLdapFilter3Scope->itemData( ui->comboBoxLdapFilter3Scope->currentIndex() ).toInt() );
    filter.filter = ui->inputLdapFilter3SearchFilter->text();

    return filter;
}

QList<quint32> UserGroupEditor::getChoosedSystemGroups() {

    // Alle Checkboxen durchlaufen und schauen, welche aktiviert sind...
    QList<quint32> list;
    QHash<QCheckBox*, quint32>::iterator it;
    for (it = _sysGroupsCheckbox.begin(); it != _sysGroupsCheckbox.end(); it++) {

        if ( it.key()->isChecked() ) {

            list.append( it.value() );
        }
    }

    return list;
}

void UserGroupEditor::setChoosedSystemGroups(const QList<quint32> &l) {

    QHash<QCheckBox*, quint32>::iterator it;
    for (it = _sysGroupsCheckbox.begin(); it != _sysGroupsCheckbox.end(); it++) {

        if ( l.contains( it.value() ) ) {

            it.key()->setCheckState( Qt::Checked );
        }
        else {
            it.key()->setCheckState( Qt::Unchecked );
        }
    }
}

void UserGroupEditor::hideGroupDeleteButton() {
    ui->pushDeleteGroup->hide();
}

void UserGroupEditor::testLDAPMembershipSettings() {

    bool ok;
    QString username = QInputDialog::getText(this, "LDAP- Einstellungen überprüfen",
                          "Bitte gib einen Benutzernamen an, mit welchem die Zuordnung getestet werden soll:",
                          QLineEdit::Normal, QString(), &ok);

    if (!ok || username.count() < 1) return;

    CREATE_TCP_COMMAND(test, administration, testLDAPGroupMemberships);
    test->setUsername(username);
    test->setLdapFilter1( getLdapFilter1() );
    test->setLdapFilter2( getLdapFilter2() );
    test->setLdapFilter3( getLdapFilter3() );
    test->setLdapFilter1Enabled( ui->groupBoxLdapFilter1->isChecked() );
    test->setLdapFilter2Enabled( ui->groupBoxLdapFilter2->isChecked() );
    test->setLdapFilter3Enabled( ui->groupBoxLdapFilter3->isChecked() );

    test->startWait();

    QString resultText;
    int matched = 0;        // Wie viele Suchfilter haben zugetroffen...
    if (test->filter1Success()) {
        resultText += "Suchfilter 1: <font color=\"#00FF00\">Suchfilter hat zugetroffen.</font><br/>";
        matched++;
    }
    else {
        resultText += "Suchfilter 1: <font color=\"#FF0000\">Suchfilter hat nicht zugetroffen.</font><br/>";
    }
    if (test->filter2Success()) {
        resultText += "Suchfilter 2: <font color=\"#00FF00\">Suchfilter hat zugetroffen.</font><br/>";
        matched++;
    }
    else {
        resultText += "Suchfilter 2: <font color=\"#FF0000\">Suchfilter hat nicht zugetroffen.</font><br/>";
    }
    if (test->filter3Success()) {
        resultText += "Suchfilter 3: <font color=\"#00FF00\">Suchfilter hat zugetroffen.</font><br/>";
        matched++;
    }
    else {
        resultText += "Suchfilter 3: <font color=\"#FF0000\">Suchfilter hat nicht zugetroffen.</font><br/>";
    }

    resultText += "<br/>";

    if (matched > 0) {
        resultText += "Benutzername '"+ username +"' würde der Gruppe zugeordnet werden.";
    }
    else {
        resultText += "Benutzername '"+ username +"' würde der Gruppe <b>_NICHT_</b> zugeordnet werden.";
    }

    resultText += "<br/>Für mehr Informationen bitte das Server- Logfile betrachten!";+

    QMessageBox::information(this, "Ergebnis - LDAP- Einstellungen überprüfen", resultText);

    qDebug() << username;
}
