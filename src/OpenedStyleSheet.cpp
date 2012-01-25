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

        // :TODO: remove test
    /*
    StyleSheet style_sheet;
    style_sheet.parse(new_text_edit->toPlainText());

    QList<StyleSheet::PropertyBlock*> test;
    style_sheet.findMatchingProperties(test,getDocumentFromTabIndex(0)->selectedElement);
    */
}

void OpenedStyleSheet::highlightString(const QString &str)
{
    highlighter->setHighlightedString(str);
}