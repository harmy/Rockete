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
    textEdit = new CodeEditor;
    textEdit->setAcceptRichText(false);
    textEdit->setLineWrapMode(QTextEdit::NoWrap);
    textEdit->setTabChangesFocus(false);

    textDocument = new QTextDocument();
    textDocument->setDefaultFont(QFont("Courier",10));
    highlighter = new CssHighlighter(textDocument);

    fillTextEdit();
}

void OpenedStyleSheet::highlightString(const QString &str)
{
    if(str.isEmpty())
    {
        highlighter->setHighlightedString("placeholder_for_highlighted_string");
    }
    else
    {
        highlighter->setHighlightedString(str);
    }
}