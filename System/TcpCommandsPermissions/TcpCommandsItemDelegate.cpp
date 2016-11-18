#include "System/TcpCommandsPermissions/TcpCommandsItemDelegate.h"

#include <QComboBox>
#include <QMap>
#include <QString>
#include <QDebug>

#include "System/TcpCommandsPermissions/TcpCommandsItem.h"
#include "System/TcpCommandsPermissions/TcpCommandsItemModel.h"

TcpCommandsItemDelegate::TcpCommandsItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}


QWidget* TcpCommandsItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                           const QModelIndex &index) const {
    QComboBox *combo = new QComboBox(parent);

    // Nur Felder, welche eine Systemgruppe beinhalten, sind bearbeitbar...
    if (index.column() == 0) return 0;

    // Systemgruppe, welche aktuell ausgewählt ist:
    int actGroup = static_cast<TcpCommandsItem*>(
                        static_cast<TcpCommandsItemModel*>( const_cast<QAbstractItemModel*>(index.model()) )->itemFromIndex(index)
                )->id();

    if (actGroup == 0) {
        combo->addItem(" - Keine Gruppe -", QVariant(0));
        combo->setCurrentIndex(0);
    }

    /**
      * Alle Systemgruppen in die ComboBox einfügen:
      */
    QMap<quint32, QString> systemGroups = static_cast<TcpCommandsItemModel*>(
                const_cast<QAbstractItemModel*>(index.model() )
                )->getAllSystemGroups();
    QMap<quint32, QString>::iterator it;
    for(it = systemGroups.begin(); it != systemGroups.end(); it++) {

        combo->addItem( it.value(), QVariant( it.key() ) );

        // Wenn es die aktuelle Systemgruppe ist, so als Standard wählen:
        if (actGroup == it.key()) {
            combo->setCurrentIndex( combo->count() - 1 );
        }
    }

    // Signal darf erst verbunden werden, nachdem alle Einträge erstellt wurden,
    // ansonste wird das Signal sofort geworfen...
    connect(combo, SIGNAL(currentIndexChanged(int)), this, SIGNAL(sysGroupValueChanged()));

    return combo;
}

void TcpCommandsItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {

}

void TcpCommandsItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index) const {

    QComboBox* combo = static_cast<QComboBox*>(editor);
    quint32 sysGroupId = combo->itemData( combo->currentIndex() ).toInt();

    // Wenn sysGroupId == "0" ist, so wurde wieder "- Keine Gruppe -" ausgewählt...
    if (sysGroupId == 0) return;

    QMap<quint32, QString> systemGroups = static_cast<TcpCommandsItemModel*>(
                const_cast<QAbstractItemModel*>(index.model() )
                )->getAllSystemGroups();
    QMap<quint32, QString>::iterator i = systemGroups.find(sysGroupId);

    // neue Systemgruppe im Item setzen:
    model->setData(index, i.value(), Qt::DisplayRole);
    model->setData(index, sysGroupId, Qt::UserRole + 1);
}

void TcpCommandsItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                                const QModelIndex &index) const {

    editor->setGeometry( option.rect );
}
