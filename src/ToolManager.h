#ifndef TOOLMANAGER_H
#define TOOLMANAGER_H

#include "Tool.h"
#include <QToolBar>
#include <QMenu>

class ToolManager
{
public:
    ToolManager();
    ~ToolManager();

    static ToolManager & getInstance() {
        static ToolManager instance;
        return instance;
    }
    Tool * getCurrentTool() const {
        return currentTool;
    }

    void initialize();
    void fillToolBar(QToolBar *tool_bar);
    void fillMenuBar(QMenu *menu);
    void changeCurrentTool(const int index);
    void changeCurrentTool(Tool *tool);
private:
    QList<Tool*> toolList;
    Tool *currentTool;
};


#endif