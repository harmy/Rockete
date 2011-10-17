#include "ActionSetAttribute.h"

ActionSetAttribute::ActionSetAttribute(OpenedDocument *document, Element *element, const QString &_attributeName, const QString &current_value, const QString &new_value)
{
    type = ActionTypeSetAttribute;
    targetElement = element;
    targetDocument = document;
    oldValue = current_value;
    newValue = new_value;
    attributeName = _attributeName;
}

ActionSetAttribute::ActionSetAttribute(OpenedDocument *document, Element *element, const QString &_attributeName, const QString &new_value)
{
    type = ActionTypeSetAttribute;
    targetElement = element;
    targetDocument = document;
    // :TODO: get current value into oldValue;
    newValue = new_value;
    attributeName = _attributeName;
}

void ActionSetAttribute::apply()
{
    targetElement->SetAttribute(attributeName.toStdString().c_str(), newValue.toStdString().c_str());
    targetDocument->regenerateBodyContent();
}

void ActionSetAttribute::unapply()
{
    targetElement->SetAttribute(attributeName.toStdString().c_str(), oldValue.toStdString().c_str());
    targetDocument->regenerateBodyContent();
}
