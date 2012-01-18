#ifndef OPENEDSTYLESHEET_H
#define OPENEDSTYLESHEET_H

#include "RocketHelper.h"
#include "OpenedFile.h"
#include "CSSHighlighter.h"

class OpenedStyleSheet : public OpenedFile
{
public:
    OpenedStyleSheet();
    void initialize();
    //virtual void highlightString(const QString &str);
    
    CssHighlighter *highlighter;
};

#endif
