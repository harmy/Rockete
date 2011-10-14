#ifndef ACTIONSETINLINEPROPERTY_H
#define ACTIONSETINLINEPROPERTY_H

#include "Action.h"

class ActionSetInlineProperty : public Action
{
public:
    ActionSetInlineProperty(OpenedDocument *document, Element *element, const QString &property_name, const QString &new_value);
    virtual void apply();
    virtual void unapply();

private:
    QString propertyName;
    OpenedDocument *targetDocument;
    bool propertyExists;
};

#endif
