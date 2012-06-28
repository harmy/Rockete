#include "OpenedLuaScript.h"

#include <QFile>
#include <QTextDocument>
#include "CodeEditor.h"

// Public:

OpenedLuaScript::OpenedLuaScript()
{
}

void OpenedLuaScript::initialize()
{
    OpenedFile::initialize();
    highlighter = new LuaHighlighter(document());

    fillTextEdit();
}

void OpenedLuaScript::highlightString(const QString &str)
{
    if(str.isEmpty())
    {
        ((LuaHighlighter *)highlighter)->setHighlightedString("placeholder_for_highlighted_string");
    }
    else
    {
        ((LuaHighlighter *)highlighter)->setHighlightedString(str);
    }

}