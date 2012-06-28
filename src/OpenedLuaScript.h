#ifndef OPENEDLUASCRIPT_H
#define OPENEDLUASCRIPT_H

#include "RocketHelper.h"
#include "OpenedFile.h"
#include "LuaHighlighter.h"

class OpenedLuaScript : public OpenedFile
{
    Q_OBJECT
public:
    OpenedLuaScript();
    virtual void initialize();
    virtual void highlightString(const QString &str);
};

#endif
