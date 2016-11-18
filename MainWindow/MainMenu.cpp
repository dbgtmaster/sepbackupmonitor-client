#include "MainMenu.h"

#include "MainWindow/MdiAbstractWindowFactory.h"

MainMenu::MainMenu(QWidget *parent) :
    QWidget(parent), _layout(this)
{


}

void MainMenu::addMenuEntry(const QString &name, MdiAbstractWindowFactory *factory) {

    QPushButton *button = new QPushButton(name, this);
    _layout.addWidget(button);

    //widget->setParent(this);
    _hashButtonToFactory.insert(button, factory);

    connect(button, SIGNAL(clicked()), this, SLOT(buttonPressed()));
}


void MainMenu::openMenuEntry(const QString &name) {

    QHash<QPushButton*, MdiAbstractWindowFactory*>::Iterator it;
    for (it = _hashButtonToFactory.begin(); it != _hashButtonToFactory.end(); it++) {
        if (it.key()->text() == name) {
            it.key()->click();
            break;
        }
    }
}

void MainMenu::buttonPressed() {

    // Das anzuzeigende Widget anhand des gedrückten Buttons suchen...
    MdiAbstractWindowFactory *factory = _hashButtonToFactory.value( static_cast<QPushButton*>(sender()) );

    // Wenn Fenster bereits geöffnet ist, so schieben wir das geöffnete Fenster in den Vordergrund:
    if (_hashFactoryToWindow.contains(factory)) {
        _hashFactoryToWindow.value(factory)->show();
        _hashFactoryToWindow.value(factory)->setFocus();
    }
    else {
        // Neues Fenster öffnen:
        MdiAbstractWindow *window = factory->createWindow();
        _hashFactoryToWindow.insert(factory, window);
        connect(window, SIGNAL(destroyed()), this, SLOT(mdiWindowClosed()));
        MainWindow::getInstance()->addMdiSubWindow(window);
    }
}

void MainMenu::mdiWindowClosed() {
    _hashFactoryToWindow.remove( _hashFactoryToWindow.key( static_cast<MdiAbstractWindow*>(sender())) );
}
