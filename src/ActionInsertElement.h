#ifndef ACTIONINSERTELEMENT_H
#define ACTIONINSERTELEMENT_H

#include "Action.h"
#include <QList>
#include "RocketHelper.h"
class OpenedDocument;

class ActionInsertElement : public Action
{
public:
    ActionInsertElement(OpenedDocument *document, Element *element, Element *_elementToInsert);
    virtual void apply();
    virtual void unapply();

private:
    OpenedDocument *targetDocument;
    Element *targetElement;
    Element *elementToInsert;
};

#endif
