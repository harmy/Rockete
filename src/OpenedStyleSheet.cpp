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

void OpenedStyleSheet::addStyleSheetTextAtEnd(const QString &new_content)
{
    QTextCursor replacingCursor = textEdit->textCursor();
    replacingCursor.movePosition(QTextCursor::End);
    replacingCursor.insertText("\n");
    replacingCursor.insertText(new_content);
    textEdit->setTextCursor(replacingCursor);
    save();
}