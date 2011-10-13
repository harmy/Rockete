#include "Action.h"

#include "OpenedFile.h"
#include "Rockete.h"
class ElementStyle;

Action::Action(OpenedDocument * document, Element * element, const QString & variable_name, const QString & current_value, const QString & new_value)
{
    type = ActionSetAttribute;
    targetElement = element;
    targetFile = document;
    oldValue = current_value;
    newValue = new_value;
    variableName = variable_name;
    addedLineNumber = -1;
}

Action::Action(OpenedDocument *document, Element *element, PropertyTreeModel::Property * property, const QString &current_value, const QString &new_value)
{
    type = ActionSetProperty;
    targetElement = element;
    targetFile = document;
    oldValue = current_value;
    newValue = new_value;
    targetProperty = property;
    addedLineNumber = -1;
}

Action::Action(OpenedDocument *document, Element *element, Element *element_to_insert)
{
    type = ActionInsertElement;
    targetElement = element;
    targetFile = document;
    elementToInsert = element_to_insert;
    addedLineNumber = -1;
}

void Action::apply()
{
    switch(type)
    {
    case ActionSetProperty:
        {
            if(targetProperty->sourceFile == "inline")
            {
                targetProperty->value = newValue;
                RocketHelper::replaceInlinedProperty(targetElement,targetProperty->name, newValue);
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

                    source_line_number = opened_file->findLineNumber(targetProperty->name, targetProperty->sourceLineNumber);
                    new_line = "    " + targetProperty->name + ": " + newValue + ";";

                    if(source_line_number != -1)
                    {
                        opened_file->replaceLine(source_line_number, new_line);
                    }
                    else
                    {
                        addedLineNumber = opened_file->insertLineBeforeBracket(targetProperty->sourceLineNumber, new_line);
                    }

                    targetProperty->value = newValue;

                    opened_file->save();
                    Rockete::getInstance().reloadCurrentDocument();
                }
            }


        }
        break;

    case ActionSetAttribute:
        targetElement->SetAttribute(variableName.toStdString().c_str(), newValue.toStdString().c_str());
        dynamic_cast<OpenedDocument*>(targetFile)->regenerateBodyContent();
        break;

    case ActionInsertElement:
        targetElement->AppendChild(elementToInsert);
        dynamic_cast<OpenedDocument*>(targetFile)->regenerateBodyContent();
        Rockete::getInstance().repaintRenderingView();
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
                targetProperty->value = oldValue;
                RocketHelper::replaceInlinedProperty(targetElement,targetProperty->name, oldValue);
                dynamic_cast<OpenedDocument*>(targetFile)->regenerateBodyContent();
                Rockete::getInstance().repaintRenderingView();
            }
            else
            {
                OpenedFile *opened_file = Rockete::getInstance().getOpenedFile(targetProperty->sourceFile.toStdString().c_str(), true);

                if(opened_file)
                {
                    if(addedLineNumber!= -1)
                    {
                        opened_file->removeLine(addedLineNumber);
                    }
                    else
                    {
                        int source_line_number;
                        QString new_line;

                        source_line_number = opened_file->findLineNumber(targetProperty->name,targetProperty->sourceLineNumber);
                        new_line = "    " + targetProperty->name + ": " + oldValue + ";";

                        opened_file->replaceLine(source_line_number, new_line);

                        targetProperty->value = oldValue;
                    }

                    opened_file->save();
                    Rockete::getInstance().reloadCurrentDocument();
                }
            }
        }
        break;

    case ActionSetAttribute:
        targetElement->SetAttribute(variableName.toStdString().c_str(), oldValue.toStdString().c_str());
        dynamic_cast<OpenedDocument*>(targetFile)->regenerateBodyContent();
        break;

    case ActionInsertElement:
        targetElement->RemoveChild(elementToInsert);
        if(dynamic_cast<OpenedDocument*>(targetFile)->selectedElement == elementToInsert)
        {
            dynamic_cast<OpenedDocument*>(targetFile)->selectedElement = NULL;
        }
        dynamic_cast<OpenedDocument*>(targetFile)->regenerateBodyContent();
        Rockete::getInstance().repaintRenderingView();
        break;

    default:
        break;
    }
}
