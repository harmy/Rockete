#include "Tool.h"

#include "ToolManager.h"
#include "Rockete.h"

Tool::Tool()
: QObject(), widget(NULL)
{

}

Tool::~Tool()
{

}

void Tool::onSelect()
{
    static QWidget *last_added_widget = NULL;

    if(last_added_widget)
    {
        last_added_widget->setParent(0);
        Rockete::getInstance().getCurrentToolTab()->layout()->removeWidget(last_added_widget);
        last_added_widget = NULL;
    }

    if(widget)
    {
        last_added_widget = widget;
        Rockete::getInstance().getCurrentToolTab()->layout()->addWidget(widget);
    }
}

void Tool::onActionTriggered()
{
    ToolManager::getInstance().changeCurrentTool(this);
}