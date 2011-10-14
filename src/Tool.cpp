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

    if (last_added_widget) {
        last_added_widget->setParent(0);
        Rockete::getInstance().getCurrentToolTab()->layout()->removeWidget(last_added_widget);
        last_added_widget = NULL;
    }

    if (widget) {
        last_added_widget = widget;
        Rockete::getInstance().getCurrentToolTab()->layout()->addWidget(widget);
    }

    Rockete::getInstance().unselectElement();
    Rockete::getInstance().repaintRenderingView();
}

// Public slots:

void Tool::onActionTriggered()
{
    ToolManager::getInstance().changeCurrentTool(this);
}

// Protected:

void Tool::renderMarkers() const
{
    foreach (const Marker &marker, markerList) {
        GraphicSystem::drawBox(marker.position - marker.extent * 0.5f, marker.extent, Color4b(255,255,255,255), true);
        GraphicSystem::drawBox(marker.position - marker.extent * 0.5f, marker.extent, Color4b(0,0,255,255), false);
    }
}

bool Tool::findMarkerFromPosition(int& markerIndex, const Vector2f &position) const
{
    for (markerIndex=0; markerIndex<markerList.size(); ++markerIndex) {
        const Vector2f & markerPosition = markerList[markerIndex].position;
        Vector2f halfExtent = markerList[markerIndex].extent * 0.5f;


        if (position.x > markerPosition.x - halfExtent.x 
            && position.x < markerPosition.x + halfExtent.x
            && position.y > markerPosition.y - halfExtent.y 
            && position.y < markerPosition.y + halfExtent.y
            )
        {
            return true;
        }
    }

    return false;
}