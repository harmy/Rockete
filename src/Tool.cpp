#include "Tool.h"
#include "ToolManager.h"

Tool::Tool()
: QObject(), widget(NULL)
{

}

Tool::~Tool()
{

}

void Tool::onSelect()
{
    if(widget)
    {
        //:TODO: Add widget to current tool tab.
    }
}

void Tool::onActionTriggered()
{
    ToolManager::getInstance().changeCurrentTool(this);
}