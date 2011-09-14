#include "Action.h"


Action::Action(OpenedDocument * document, Element * element, const QString & variable_name, const QString & current_value, const QString & new_value)
{
    type = ActionSetAttribute;
    targetElement = element;
    targetFile = document;
    oldValue = current_value;
    newValue = new_value;
    variableName = variable_name;
}

void Action::apply()
{
    switch(type)
    {
    case ActionSetProperty:
        {
            // :TODO:
        }
        break;
    case ActionSetAttribute:
        targetElement->SetAttribute(variableName.toStdString().c_str(), newValue.toStdString().c_str());
        break;
    default:
        break;
    }
}

void Action::unapply()
{
    switch(type)
    {
    case ActionSetAttribute:
        targetElement->SetAttribute(variableName.toStdString().c_str(), oldValue.toStdString().c_str());
        break;
    default:
        break;
    }
}
