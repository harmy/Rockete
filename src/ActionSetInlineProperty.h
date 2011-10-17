#ifndef ACTIONSETINLINEPROPERTY_H
#define ACTIONSETINLINEPROPERTY_H

#include "Action.h"
#include "OpenedDocument.h"
#include <QString>

class ActionSetInlineProperty : public Action
{
public:
    ActionSetInlineProperty(OpenedDocument *document, Element *element, const QString &property_name, const QString &new_value);
    virtual void apply();
    virtual void unapply();

private:
    QString newValue;
    QString oldValue;
    QString propertyName;
    OpenedDocument *targetDocument;
    Element *targetElement;
    bool propertyExists;
};

#endif
