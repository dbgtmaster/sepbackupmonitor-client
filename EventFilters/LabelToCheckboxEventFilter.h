/**
  * Dieser Event- Filter verbindet ein QLabel mit einer QCheckbox.
  * Wird auf dieses Label geklickt, so wird der QCheckbox aktiviert bzw. deaktiviert.
  */

#ifndef LABELTOCHECKBOXEVENTFILTER_H
#define LABELTOCHECKBOXEVENTFILTER_H

#include <QObject>

class QCheckBox;

class LabelToCheckboxEventFilter : public QObject
{
    Q_OBJECT
private:
    QCheckBox *_checkbox;
public:
    explicit LabelToCheckboxEventFilter(QCheckBox *checkbox, QObject *parent = 0);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

signals:

public slots:

};

#endif // LABELTOCHECKBOXEVENTFILTER_H
