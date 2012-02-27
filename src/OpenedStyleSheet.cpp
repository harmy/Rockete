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
    textEdit = new CodeEditor(this);
    //textEdit->setAcceptRichText(false);
    textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
    textEdit->setTabChangesFocus(false);

    textDocument = new QTextDocument();
    textDocument->setDefaultFont(QFont("Courier",10));
    textDocument->setDocumentLayout(new QPlainTextDocumentLayout(textDocument));
    highlighter = new CssHighlighter(textDocument);

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