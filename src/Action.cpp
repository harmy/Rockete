#include "Action.h"

#include "OpenedFile.h"
#include "Rockete.h"

Action::Action(OpenedDocument * document, Element * element, const QString & variable_name, const QString & current_value, const QString & new_value)
{
    type = ActionSetAttribute;
    targetElement = element;
    targetFile = document;
    oldValue = current_value;
    newValue = new_value;
    variableName = variable_name;
}

Action::Action(OpenedDocument *document, Element *element, PropertyTreeModel::Property * property, const QString &current_value, const QString &new_value)
{
    type = ActionSetProperty;
    targetElement = element;
    targetFile = document;
    oldValue = current_value;
    newValue = new_value;
    targetProperty = property;
}

void Action::apply()
{
    switch(type)
    {
    case ActionSetProperty:
        {
            if(targetProperty->sourceFile == "inline")
            {
                targetElement->SetProperty(targetProperty->name.toStdString().c_str(),newValue.toStdString().c_str());

                dynamic_cast<OpenedDocument*>(targetFile)->regenerateBodyContent();

                Rockete::getInstance().repaintRenderingView();
            }
            else
            {
                OpenedFile *opened_file = Rockete::getInstance().getOpenedFile(targetProperty->sourceFile.toStdString().c_str(), true);

                if(opened_file)
                {
                    int source_line_number;
                    QString new_line;

                    source_line_number = opened_file->findLineNumber(targetProperty->name,targetProperty->sourceLineNumber);
                    new_line = "    " + targetProperty->name + ": " + newValue + ";";

                    opened_file->replaceLine(source_line_number, new_line);

                    targetProperty->value = newValue;

                    opened_file->save();
                    Rockete::getInstance().reloadCurrentDocument();
                }
            }


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
    case ActionSetProperty:
        {
            if(targetProperty->sourceFile == "inline")
            {
                targetElement->SetProperty(targetProperty->name.toStdString().c_str(),oldValue.toStdString().c_str());
                Rockete::getInstance().repaintRenderingView();
            }
            else
            {
                OpenedFile *opened_file = Rockete::getInstance().getOpenedFile(targetProperty->sourceFile.toStdString().c_str(), true);

                if(opened_file)
                {
                    int source_line_number;
                    QString new_line;

                    source_line_number = opened_file->findLineNumber(targetProperty->name,targetProperty->sourceLineNumber);
                    new_line = "    " + targetProperty->name + ": " + oldValue + ";";

                    opened_file->replaceLine(source_line_number, new_line);

                    targetProperty->value = oldValue;

                    opened_file->save();
                    Rockete::getInstance().reloadCurrentDocument();
                }
            }
        }
        break;
    case ActionSetAttribute:
        targetElement->SetAttribute(variableName.toStdString().c_str(), oldValue.toStdString().c_str());
        break;
    default:
        break;
    }
}
