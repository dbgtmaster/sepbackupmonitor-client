#ifndef MAINMENU_H
#define MAINMENU_H

#include <QObject>
#include <QWidget>
#include <QLayout>
#include "QPushButton"

#include <MainWindow/MainWindow.h>

class MdiAbstractWindowFactory;
class MdiAbstractWindow;

class MainMenu : public QWidget
{
    Q_OBJECT
private:
    QHBoxLayout _layout;

    // Eine Liste, welcher Button wellchem Factory zugeordnet ist.
    QHash<QPushButton*, MdiAbstractWindowFactory*> _hashButtonToFactory;

    // Eine Liste, welcher Factory welches Fenster erstellt hat:
    QHash<MdiAbstractWindowFactory*, MdiAbstractWindow*> _hashFactoryToWindow;

public:
    explicit MainMenu(QWidget *parent = 0);

    // Fügt einen neuen Eintrag ins Menü hinzu...
    void addMenuEntry(const QString &name, MdiAbstractWindowFactory *factory);

    void openMenuEntry(const QString &name);

signals:

public slots:

    // Slot wird aufgerufen, wenn ein Button gedrückt wurde:
    void buttonPressed();

    // Wenn ein SUBFenster geschlossen wurde.
    void mdiWindowClosed();
};

#endif // MAINMENU_H
