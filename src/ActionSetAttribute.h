#ifndef ACTIONSETATTRIBUTE_H
#define ACTIONSETATTRIBUTE_H

#include "Action.h"
#include "OpenedDocument.h"
#include <QString>

class ActionSetAttribute : public Action
{
public:
    ActionSetAttribute(OpenedDocument *document, Element *element, const QString &variable_name, const QString &current_value, const QString &new_value);
    ActionSetAttribute(OpenedDocument *document, Element *element, const QString &variable_name, const QString &new_value);
    virtual void apply();
    virtual void unapply();

private:
    QString newValue;
    QString oldValue;
    QString attributeName;
    OpenedDocument *targetDocument;
    Element *targetElement;
};

#endif
