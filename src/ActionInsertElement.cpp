#include "ActionInsertElement.h"

#include "Rockete.h"
#include "OpenedDocument.h"
#include "RocketHelper.h"

ActionInsertElement::ActionInsertElement(OpenedDocument *document, Element *element, Element *element_to_insert)
{
    type = ActionTypeInsertElement;
    targetElement = element;
    targetDocument = document;
    elementToInsert = element_to_insert;
}

void ActionInsertElement::apply()
{
    targetElement->AppendChild(elementToInsert);
    targetDocument->regenerateBodyContent();
    Rockete::getInstance().repaintRenderingView();
}

void ActionInsertElement::unapply()
{
    targetElement->RemoveChild(elementToInsert);

    if (targetDocument->selectedElement == elementToInsert)
        targetDocument->selectedElement = NULL;

    targetDocument->regenerateBodyContent();
    Rockete::getInstance().repaintRenderingView();
}

