#include "ToolManager.h"

#include "ToolSelecter.h"
#include "ToolDiv.h"

ToolManager::ToolManager()
{

}

ToolManager::~ToolManager()
{

}

void ToolManager::initialize()
{
    toolList.push_back(new ToolSelecter());
    toolList.push_back(new ToolDiv());
}

void ToolManager::setup(QToolBar *tool_bar,QMenu *menu)
{
    QString shortcut_string;
    Tool *tool;

    for(int tool_index=0; tool_index<toolList.size(); ++tool_index)
    {
        tool = toolList[tool_index];

        tool->setAction(tool_bar->addAction(QIcon(tool->getImageName()), tool->getName(), tool, SLOT(onActionTriggered())));

        menu->addAction(tool->getAction());

        shortcut_string = "F" + QString::number(tool_index+1);

        tool->getAction()->setShortcut(QKeySequence(shortcut_string));
        tool->getAction()->setCheckable(true);
    }

    changeCurrentTool(0);
}

void ToolManager::changeCurrentTool(const int index)
{
    Q_ASSERT(index < toolList.size());

    if(currentTool == toolList[index])
    {
        return;
    }

    foreach(Tool *tool, toolList)
    {
        tool->getAction()->setChecked(false);
    }

    currentTool = toolList[index];
    currentTool->onSelect();
    currentTool->getAction()->setChecked(true);
}

void ToolManager::changeCurrentTool(Tool *tool)
{
    changeCurrentTool(toolList.indexOf(tool));
}
