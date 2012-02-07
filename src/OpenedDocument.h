#ifndef OPENEDDOCUMENT_H
#define OPENEDDOCUMENT_H

#include "RocketHelper.h"
#include "OpenedFile.h"
#include "StyleSheet.h"
#include "XMLHighlighter.h"
#include <QList>

class OpenedDocument : public OpenedFile
{
public:
    OpenedDocument();

    virtual void initialize();
    void replaceInnerRMLFromId(const QString &tag_name, const QString &id, const QString &new_content);
    void replaceInnerRMLFromTagName(const QString &tag_name, const QString &new_content);
    void regenerateBodyContent();
    virtual void highlightString(const QString &str);

    RMLDocument *rocketDocument;
    Element *selectedElement;
    QList<StyleSheet*> styleSheetList;
    XMLHighlighter *highlighter;
    QString highlightedString;
};

#endif
