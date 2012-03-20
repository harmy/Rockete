#include "DocumentHierarchyEventFilter.h"

#include <QMouseEvent>

bool DocumentHierarchyEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        ShiftPressed = false;

        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if(mouseEvent->modifiers() & Qt::ShiftModifier) {
            ShiftPressed = true;
        }

        return QObject::eventFilter(obj, event);
    } else {
        return QObject::eventFilter(obj, event);
    }
}