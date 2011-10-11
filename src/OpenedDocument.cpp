#include "OpenedDocument.h"

#include "XMLHighlighter.h"

OpenedDocument::OpenedDocument() : selectedElement(NULL)
{

}

void OpenedDocument::initialize()
{
    textEdit = new QTextEdit;
    textEdit->setAcceptRichText(false);
    textEdit->setFontFamily("Courier");
    textEdit->setLineWrapMode(QTextEdit::NoWrap);

    textDocument = new QTextDocument();
    textDocument->setDefaultFont(QFont("Courier",10));
    new XMLHighlighter(textDocument);

    fillTextEdit();
}
