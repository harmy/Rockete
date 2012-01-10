#include "ActionSetAttribute.h"

#include "Rockete.h"

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
    oldValue = element->GetAttribute<Rocket::Core::String>(_attributeName.toAscii().data(), "").CString();
    newValue = new_value;
    attributeName = _attributeName;
}

void ActionSetAttribute::apply()
{
    targetElement->SetAttribute(attributeName.toAscii().data(), newValue.toAscii().data());
    targetDocument->regenerateBodyContent();
    Rockete::getInstance().repaintRenderingView();
}

void ActionSetAttribute::unapply()
{
    targetElement->SetAttribute(attributeName.toAscii().data(), oldValue.toAscii().data());
    targetDocument->regenerateBodyContent();
    Rockete::getInstance().repaintRenderingView();
}
