#ifndef ACTION_H
#define ACTION_H

#include <QString>
#include "RocketHelper.h"
#include "OpenedFile.h"
#include "OpenedDocument.h"
#include "OpenedStyleSheet.h"
#include "PropertyTreeModel.h"

// :TODO: Make Action class abstract, create Action* classes.

class Action
{
public:
    enum Type
    {
        ActionTypeUnknown,
        ActionTypeSetProperty,
        ActionTypeSetInlineProperty,
        ActionTypeSetAttribute,
        ActionTypeInsertElement,
        ActionTypeGroup
    };
    Action();
    Action(OpenedDocument *document, Element *element, PropertyTreeModel::Property * property, const QString &current_value, const QString &new_value);
    Action(OpenedDocument *document, Element *element, const QString &variable_name, const QString &current_value, const QString &new_value);
    Action(OpenedDocument *document, Element *element, Element *element_to_insert);
    virtual ~Action();
    Type getType() const { return type; };
    virtual void apply();
    virtual void unapply();

protected:
    QString variableName;
    QString oldValue;
    QString newValue;
    OpenedFile *targetFile;
    Element *targetElement;
    Element *elementToInsert;
    PropertyTreeModel::Property * targetProperty;
    Type type;
    int addedLineNumber;
};

#endif
