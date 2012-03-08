#include "OpenedStyleSheet.h"

#include <QFile>
#include <QTextDocument>
#include "CodeEditor.h"

// Public:

OpenedStyleSheet::OpenedStyleSheet()
{
}

void OpenedStyleSheet::initialize()
{
    OpenedFile::initialize();
    highlighter = new CssHighlighter(document());

    fillTextEdit();
}

void OpenedStyleSheet::highlightString(const QString &str)
{
    if(str.isEmpty())
    {
        ((CssHighlighter *)highlighter)->setHighlightedString("placeholder_for_highlighted_string");
    }
    else
    {
        ((CssHighlighter *)highlighter)->setHighlightedString(str);
    }
}

void OpenedStyleSheet::addStyleSheetTextAtEnd(const QString &new_content)
{
    QTextCursor replacingCursor = textCursor();
    replacingCursor.movePosition(QTextCursor::End);
    replacingCursor.insertText("\n");
    replacingCursor.insertText(new_content);
    setTextCursor(replacingCursor);
    save();
}