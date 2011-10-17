#ifndef ACTIONSETPROPERTY_H
#define ACTIONSETPROPERTY_H

#include "Action.h"

#include "OpenedDocument.h"
#include "PropertyTreeModel.h"
#include <QString>

class ActionSetProperty : public Action
{
public:
    ActionSetProperty(OpenedDocument *document, Element *element, PropertyTreeModel::Property *property, const QString &new_value);
    virtual void apply();
    virtual void unapply();

private:
    QString newValue;
    QString oldValue;
    QString attributeName;
    OpenedDocument *targetDocument;
    Element *targetElement;
    PropertyTreeModel::Property * targetProperty;
    int addedLineNumber;
};

#endif
