#ifndef CUSTOMERADMINISTRATION_H
#define CUSTOMERADMINISTRATION_H

#include "MainWindow/MdiAbstractWindow.h"

class QModelIndex;

namespace Ui {
class CustomerAdministration;
}

class QStandardItemModel;

class CustomerAdministration : public MdiAbstractWindow
{
    Q_OBJECT
    
private:
    QStandardItemModel *_customerTableModel;

    // Ob die Grunddaten der Kunden bearbeitbar sind, bzw. Kunden erstellt werden können.
    bool _customerDataEditable;

public:
    explicit CustomerAdministration(QWidget *parent = 0);
    ~CustomerAdministration();
    
public slots:
    void search();
    void create();

    void customerClicked(const QModelIndex &);

private:
    Ui::CustomerAdministration *ui;
};

#endif // CUSTOMERADMINISTRATION_H
