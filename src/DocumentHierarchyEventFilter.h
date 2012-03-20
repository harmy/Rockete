#ifndef DOCUMENTHIERARCHYEVENTFILTER_H
#define DOCUMENTHIERARCHYEVENTFILTER_H

#include <QObject>
#include <QEvent>

class DocumentHierarchyEventFilter : public QObject
{
public:
    bool ShiftPressed;
protected:
    bool eventFilter(QObject *obj, QEvent *event);

};

#endif
