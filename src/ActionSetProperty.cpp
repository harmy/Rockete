#include "ActionSetProperty.h"

#include "Rockete.h"
#include "OpenedDocument.h"
#include "PropertyTreeModel.h"

ActionSetProperty::ActionSetProperty(OpenedDocument *document, Element *element, PropertyTreeModel::Property *property, const QString &new_value)
{
    type = ActionTypeSetProperty;
    targetElement = element;
    targetDocument = document;
    oldValue = property->value;
    newValue = new_value;
    targetProperty = property;
    addedLineNumber = -1;
}


void ActionSetProperty::apply()
{
    if (targetProperty->sourceFile == "inline") {
        targetProperty->value = newValue;
        RocketHelper::replaceInlinedProperty(targetElement,targetProperty->name, newValue);
        targetDocument->regenerateBodyContent();
        Rockete::getInstance().repaintRenderingView();
    }
    else {
        OpenedFile *opened_file = Rockete::getInstance().getOpenedFile(targetProperty->sourceFile.toStdString().c_str(), true);

        if (opened_file) {
            int source_line_number;
            QString new_line;

            source_line_number = opened_file->findLineNumber(targetProperty->name, targetProperty->sourceLineNumber);
            new_line = "    " + targetProperty->name + ": " + newValue + ";";

            if(source_line_number != -1)
                opened_file->replaceLine(source_line_number, new_line);
            else
                addedLineNumber = opened_file->insertLineBeforeBracket(targetProperty->sourceLineNumber, new_line);

            targetProperty->value = newValue;

            opened_file->save();
            Rockete::getInstance().reloadCurrentDocument();
        }
    }
}

void ActionSetProperty::unapply()
{
    if (targetProperty->sourceFile == "inline") {
        targetProperty->value = oldValue;
        RocketHelper::replaceInlinedProperty(targetElement,targetProperty->name, oldValue);
        targetDocument->regenerateBodyContent();
        Rockete::getInstance().repaintRenderingView();
    }
    else {
        OpenedFile *opened_file = Rockete::getInstance().getOpenedFile(targetProperty->sourceFile.toStdString().c_str(), true);

        if (opened_file) {
            if (addedLineNumber!= -1)
                opened_file->removeLine(addedLineNumber);
            else {
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
