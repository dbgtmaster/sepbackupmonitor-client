#ifndef TCPCOMMANDSITEMDELEGATE_H
#define TCPCOMMANDSITEMDELEGATE_H

#include <QItemDelegate>

class TcpCommandsItemDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    explicit TcpCommandsItemDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;

public slots:

signals:
    // Wenn der Index der Systemgruppe geändert wurde (QCombobox)
    void sysGroupValueChanged();

};

#endif // TCPCOMMANDSITEMDELEGATE_H
