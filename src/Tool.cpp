#include "Tool.h"

#include "ToolManager.h"
#include "Rockete.h"

Tool::Tool()
: QObject(), layout(NULL)
{

}

Tool::~Tool()
{

}

void Tool::onSelect()
{
    if(layout)
    {
        Rockete::getInstance().getCurrentToolTab()->setLayout(layout);
    }
}

void Tool::onActionTriggered()
{
    ToolManager::getInstance().changeCurrentTool(this);
}