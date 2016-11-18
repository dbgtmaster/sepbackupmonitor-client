#include "ComboBoxItemDelegate.h"

#include <QComboBox>
#include <QMap>
#include <QString>

#include <QDebug>

ComboBoxItemDelegate::ComboBoxItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}


QWidget* ComboBoxItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                           const QModelIndex &index) const {
    QComboBox *combo = new QComboBox(parent);

    combo->setContentsMargins(50, 0, 5, 0);

    connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(emitCommitData()));

    // Gruppe, welche aktuell ausgewählt ist:
    int actGroup = index.model()->data(index, Qt::UserRole + 1).toInt();

    if (actGroup == 0) {
        combo->addItem(" - Keine Gruppe -", QVariant(0));
        combo->setCurrentIndex(0);
    }

    // Systemgruppen in die Combobox einfügen:
    QMap<quint32, QString>::iterator it;
    for(it = _systemGroups->begin(); it != _systemGroups->end(); it++) {

        combo->addItem( it.value(), QVariant( it.key() ) );

        // Wenn es die aktuelle Systemgruppe ist, so als Standard wählen:
        if (actGroup == it.key()) {
            combo->setCurrentIndex( combo->count() - 1 );
        }
    }


    return combo;
}

void ComboBoxItemDelegate::emitCommitData() {
    qDebug() << "a";
    emit commitData( qobject_cast<QWidget *>(sender()) );
}

void ComboBoxItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {

    // Signal werfen, damit Anwendung die Änderung mitprotokollieren kann..
    qDebug() << "changed!";
}

void ComboBoxItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index) const {

    QComboBox* combo = static_cast<QComboBox*>(editor);
    quint32 sysGroupId = combo->itemData( combo->currentIndex() ).toInt();

    // Wenn sysGroupId == "0" ist, so wurde wieder "- Keine Gruppe -" ausgewählt...
    if (sysGroupId == 0) return;

    QMap<quint32, QString>::iterator i = _systemGroups->find(sysGroupId);

    // neue Systemgruppe im Item setzen:
    model->setData(index, i.value(), Qt::DisplayRole);
    model->setData(index, sysGroupId, Qt::UserRole + 1);
}

void ComboBoxItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                                const QModelIndex &index) const {

    editor->setGeometry( option.rect );
}

void ComboBoxItemDelegate::setSystemGroups(QSharedPointer< QMap<quint32, QString> > &groups) {
    _systemGroups = groups;
}
