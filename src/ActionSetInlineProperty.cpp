#include "ActionSetInlineProperty.h"

#include "RocketHelper.h"
#include "Rockete.h"

ActionSetInlineProperty::ActionSetInlineProperty(OpenedDocument *document, Element *element, const QString &property_name, const QString &new_value)
{
    type = ActionTypeSetInlineProperty;
    targetElement = element;
    targetDocument = document;
    propertyName = property_name;
    newValue = new_value;
    propertyExists = RocketHelper::getInlinedProperty(oldValue, element,propertyName);
}

void ActionSetInlineProperty::apply()
{
    if (!propertyExists)
        RocketHelper::addInlinedProperty(targetElement, propertyName, newValue);
    else
        RocketHelper::replaceInlinedProperty(targetElement, propertyName, newValue);

    targetDocument->regenerateBodyContent();
    Rockete::getInstance().repaintRenderingView();
}

void ActionSetInlineProperty::unapply()
{
    if (!propertyExists)
        RocketHelper::removeInlinedProperty(targetElement, propertyName);
    else
        RocketHelper::replaceInlinedProperty(targetElement, propertyName, oldValue);

    targetDocument->regenerateBodyContent();
    Rockete::getInstance().repaintRenderingView();
}