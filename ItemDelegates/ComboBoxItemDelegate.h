#ifndef COMBOBOXITEMDELEGATE_H
#define COMBOBOXITEMDELEGATE_H

#include <QItemDelegate>

class ComboBoxItemDelegate : public QItemDelegate
{
    Q_OBJECT
private:

    // Liste mit allen Systemgruppen:
    QSharedPointer< QMap<quint32, QString > > _systemGroups;

public:
    explicit ComboBoxItemDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;

    // Übergibt die Liste mit allen verfügbaren Systemgruppen:
    // Format: QPair <SysGroup ID, SysGroup Name>
    void setSystemGroups(QSharedPointer< QMap<quint32, QString> > &groups );

public slots:
    // Signal wird geworfen, wenn sich eine Systemgruppe geändert hat...
    void emitCommitData();
};

#endif // COMBOBOXITEMDELEGATE_H
