#ifndef OPENEDDOCUMENT_H
#define OPENEDDOCUMENT_H

#include "RocketHelper.h"
#include "OpenedFile.h"

class OpenedDocument : public OpenedFile
{
public:
    OpenedDocument()
    {
        selectedElement = NULL;
    }

    RMLDocument * rocketDocument;
    Element * selectedElement;
};

#endif
