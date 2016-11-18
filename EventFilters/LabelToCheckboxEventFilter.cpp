#include "LabelToCheckboxEventFilter.h"

#include <QEvent>
#include <QMouseEvent>
#include <QCheckBox>

LabelToCheckboxEventFilter::LabelToCheckboxEventFilter(QCheckBox *checkbox, QObject *parent) :
    QObject(parent), _checkbox(checkbox)
{
}

bool LabelToCheckboxEventFilter::eventFilter(QObject *obj, QEvent *event) {

    if (event->type() == QEvent::MouseButtonPress) {

        QMouseEvent *eMouse = static_cast<QMouseEvent*>(event);
        if (eMouse->button() == Qt::LeftButton) {
            _checkbox->setChecked( !_checkbox->isChecked() );
            return true;
        }
    }

    return QObject::eventFilter(obj, event);
}
