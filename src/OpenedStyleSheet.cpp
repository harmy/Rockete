#include "OpenedStyleSheet.h"

#include <QFile>
#include <QTextDocument>
#include "CodeEditor.h"
#include "CSSHighlighter.h"

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
    new CssHighlighter(textDocument);

    fillTextEdit();

        // :TODO: remove test
    /*
    StyleSheet style_sheet;
    style_sheet.parse(new_text_edit->toPlainText());

    QList<StyleSheet::PropertyBlock*> test;
    style_sheet.findMatchingProperties(test,getDocumentFromTabIndex(0)->selectedElement);
    */
}
