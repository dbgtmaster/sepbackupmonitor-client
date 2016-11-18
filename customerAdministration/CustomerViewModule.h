/**
  * Stellt die Grundlage eines Modules zum betrachten eines Kunden dar..
  */

#ifndef CUSTOMERVIEWMODULE_H
#define CUSTOMERVIEWMODULE_H

#include <QWidget>
#include <QStringList>
#include <QHash>
#include <QVariant>

class CustomerViewModule : public QWidget
{
    Q_OBJECT
public:
    explicit CustomerViewModule(QWidget *parent = 0);
    
    // Gibt eine Liste der benötigten Customer- Spalten
    virtual QStringList getNeededCustomerColumns() = 0;

    // Das Module initilisieren..
    virtual void init(const QHash<QString, QVariant> &row) = 0;

    // Geänderten Spalten des Kunden werden zurückgegeben, welche gespeichert werden sollen..
    // Die Standardversion tut gar nichts, sollte daher vom Modul überladen werden.
    virtual QHash<QString, QVariant> save();

signals:
    
};

#endif // CUSTOMERVIEWMODULE_H
