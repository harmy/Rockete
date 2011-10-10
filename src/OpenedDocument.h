#ifndef OPENEDDOCUMENT_H
#define OPENEDDOCUMENT_H

#include "RocketHelper.h"
#include "OpenedFile.h"
#include "StyleSheet.h"
#include <QList>

class OpenedDocument : public OpenedFile
{
public:
    OpenedDocument()
    {
        selectedElement = NULL;
    }

    RMLDocument * rocketDocument;
    Element * selectedElement;
    QList<StyleSheet*> styleSheetList;
};

#endif
