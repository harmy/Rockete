#include "OpenedDocument.h"

#include "RocketSystem.h"
#include "CodeEditor.h"

OpenedDocument::OpenedDocument() : selectedElement(NULL)
{
}

void OpenedDocument::initialize()
{
    textEdit = new CodeEditor();
    textEdit->setAcceptRichText(false);
    textEdit->setLineWrapMode(QTextEdit::NoWrap);
    textEdit->setTabChangesFocus(false);

    textDocument = new QTextDocument();
    textDocument->setDefaultFont(QFont("Courier",10));
    highlighter = new XMLHighlighter(textDocument);

    fillTextEdit();
}

void OpenedDocument::replaceInnerRMLFromId(const QString &tag_name, const QString &id, const QString &new_content)
{
    QString content;
    QString id_string;
    QString end_string;
    int id_index;
    int start_index;
    int end_index;

    content = textDocument->toPlainText();

    id_string = "id=\"" + id + "\"";
    end_string = "</" + tag_name + ">";

    id_index = content.indexOf(id_string);

    Q_ASSERT(id_index != -1);

    start_index = content.indexOf('>',id_index);
    end_index = content.indexOf(end_string,start_index+1);

    content.replace(start_index+1, end_index-start_index-1,new_content);

    setTextEditContent(content);
}

void OpenedDocument::replaceInnerRMLFromTagName(const QString &tag_name, const QString &new_content)
{
    QString content;
    QString tag;
    QString end_string;
    int tag_index;
    int start_index;
    int end_index;

    tag = "<" + tag_name;

    content = textDocument->toPlainText();
    end_string = "</" + tag_name + ">";
    tag_index = content.indexOf(tag);

    Q_ASSERT(tag_index != -1);

    start_index = content.indexOf('>',tag_index);
    end_index = content.indexOf(end_string,start_index+1);

    content.replace(start_index+1, end_index-start_index-1,new_content);

    setTextEditContent(content);
}

void OpenedDocument::regenerateBodyContent()
{
    Rocket::Core::String rocket_string_content;
    Element *content_element;

    RocketSystem::getInstance().getContext()->Update();

    content_element = rocketDocument->GetElementById("content");

    Q_ASSERT(content_element); // :TODO: Handle document without template.

    content_element->GetInnerRML(rocket_string_content);

    replaceInnerRMLFromTagName("body", QString(rocket_string_content.CString()));
}

void OpenedDocument::highlightString(const QString &str)
{
    highlighter->setHighlightedString(str);
}