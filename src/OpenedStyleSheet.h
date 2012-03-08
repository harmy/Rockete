#ifndef OPENEDSTYLESHEET_H
#define OPENEDSTYLESHEET_H

#include "RocketHelper.h"
#include "OpenedFile.h"
#include "CSSHighlighter.h"

class OpenedStyleSheet : public OpenedFile
{
    Q_OBJECT
public:
    OpenedStyleSheet();
    virtual void initialize();
    virtual void highlightString(const QString &str);
    void addStyleSheetTextAtEnd(const QString &new_content);
};

#endif
