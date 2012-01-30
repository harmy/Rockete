#include "ToolDiv.h"

#include "Rockete.h"
#include "RocketHelper.h"
#include "OpenedDocument.h"
#include "ActionManager.h"
#include "ActionSetInlineProperty.h"
#include "ActionInsertElement.h"
#include "ActionGroup.h"
#include <QLabel>
#include <QToolBar>
#include <QVBoxLayout>

#define MARKER_INDEX_BOTTOM_RIGHT 0
#define MARKER_INDEX_TOP_RIGHT 1

ToolDiv::ToolDiv()
: Tool(), selectedElement(NULL), itIsResizing(false), currentMarkerIndex(-1)
{
    QVBoxLayout *layout;
    QToolBar *tool_bar;

    name = "Div tool";
    imageName = ":/images/tool_div.png";

    widget = new QWidget();
    layout = new QVBoxLayout();

    layout->addWidget(new QLabel("Insert:"));
    tool_bar = new QToolBar();
    tool_bar->addAction(QIcon(), "New", this, SLOT(insertDiv()));
    
    layout->addWidget(tool_bar);

    layout->addWidget(new QLabel("Modify:"));
    tool_bar = new QToolBar();
    tool_bar->addAction(QIcon(), "Expand width", this, SLOT(expandWidth()));
    tool_bar->addAction(QIcon(), "display:inline-block", this, SLOT(setInlineBlockDisplay()));
    tool_bar->addAction(QIcon(), "To the left!", this, SLOT(setLeftAlignment()));
    tool_bar->addAction(QIcon(), "To the right!", this, SLOT(setRightAlignment()));

    layout->addWidget(tool_bar);

    widget->setLayout(layout);
}

void ToolDiv::onElementClicked(Element *_element)
{
    if (selectedElement == _element
        || itIsResizing)
        return;

    Element *element = getDivParent(_element);

    markerList.clear();

    if (element) {
        selectedElement = element;
        Rockete::getInstance().selectElement(element);
        setupMarkers();
        return;
    }

    selectedElement = NULL;
}

void ToolDiv::onRender()
{
    OpenedDocument *document;

    document = Rockete::getInstance().getCurrentDocument();

    if (document) {
        glLineWidth(2.0f);
        processElement(document->rocketDocument);

        if(document->selectedElement) {
            RocketHelper::drawBoxAroundElement(document->selectedElement, Color4b(10, 10, 240, 255));
        }

        glLineWidth(1.0f);
    }

    renderMarkers();
}

void ToolDiv::onMousePress(const Qt::MouseButton /*button*/, const Vector2f &position)
{
    if (findMarkerFromPosition(currentMarkerIndex, position)) {
        initialPosition = position;
        //itIsResizing = true;
    }
}

void ToolDiv::onMouseRelease(const Qt::MouseButton, const Vector2f &position)
{
    if (itIsResizing) {
        switch (currentMarkerIndex) {
        case MARKER_INDEX_BOTTOM_RIGHT:
            RocketHelper::incrementInlinedDimensions(Rockete::getInstance().getCurrentDocument(), selectedElement, position - initialPosition);
            break;
        case MARKER_INDEX_TOP_RIGHT:
            Vector2f difference;
            difference.x = position.x - initialPosition.x;
            difference.y = initialPosition.y - position.y;
            RocketHelper::incrementInlinedDimensions(Rockete::getInstance().getCurrentDocument(), selectedElement, difference);
            break;
        }

        markerList.clear();
        setupMarkers();
        itIsResizing = false;
    }

}

void ToolDiv::onMouseMove(const Vector2f &position)
{
    if (itIsResizing)
        markerList[currentMarkerIndex].position = position;
}

void ToolDiv::onUnselect()
{
    markerList.clear();
}

Element *ToolDiv::getDivParent(Element *element)
{
    while (element) {
        if (element->GetTagName() == "div") {
            return  element;
        }

        element = element->GetParentNode();
    }

    return NULL;
}

// Private slots:

void ToolDiv::insertDiv()
{
    OpenedDocument *document;

    document = Rockete::getInstance().getCurrentDocument();

    if (document && document->selectedElement)  
        insertDiv(document->selectedElement);
    else if(document->rocketDocument)
        insertDiv(document->rocketDocument);
}

void ToolDiv::expandWidth()
{
    OpenedDocument *document;

    document = Rockete::getInstance().getCurrentDocument();

    if (document && document->selectedElement) {
        ActionManager::getInstance().applyNew(new ActionSetInlineProperty(document, document->selectedElement, "width", "100%"));
    }
}

void ToolDiv::setInlineBlockDisplay()
{
    OpenedDocument *document;

    document = Rockete::getInstance().getCurrentDocument();

    if (document && document->selectedElement) {
        ActionManager::getInstance().applyNew(new ActionSetInlineProperty(document, document->selectedElement, "display", "inline-block"));
    }
}

void ToolDiv::setLeftAlignment()
{
    OpenedDocument *document;

    document = Rockete::getInstance().getCurrentDocument();

    if (document && document->selectedElement) {
        ActionGroup *actionGroup = new ActionGroup();
        actionGroup->add(new ActionSetInlineProperty(document, document->selectedElement, "display", "inline-block"));
        actionGroup->add(new ActionSetInlineProperty(document, document->selectedElement, "float", "left"));
        ActionManager::getInstance().applyNew(actionGroup);
    }
}

void ToolDiv::setRightAlignment()
{
    OpenedDocument *document;

    document = Rockete::getInstance().getCurrentDocument();

    if (document && document->selectedElement) {
        ActionGroup *actionGroup = new ActionGroup();
        actionGroup->add(new ActionSetInlineProperty(document, document->selectedElement, "display", "inline-block"));
        actionGroup->add(new ActionSetInlineProperty(document, document->selectedElement, "float", "right"));
        ActionManager::getInstance().applyNew(actionGroup);
    }
}

// Private:

void ToolDiv::processElement(Element *element)
{
    if (element->GetTagName() == "div") {
        RocketHelper::drawBoxAroundElement(element, Color4b(10, 240, 10, 255));
    }

    for (int child_index=0; child_index < element->GetNumChildren(); ++child_index) {
        processElement(element->GetChild(child_index));
    }
}

void ToolDiv::insertDiv(Element *element)
{
    Rocket::Core::XMLAttributes attributes;
    attributes.Set("style", "width:50px; height:50px;");
    Element* div = Rocket::Core::Factory::InstanceElement(NULL, "div", "div", attributes);

    ActionManager::getInstance().applyNew(new ActionInsertElement(Rockete::getInstance().getCurrentDocument(), element, div));
    Rockete::getInstance().selectElement(div);
}

void ToolDiv::setupMarkers()
{
    OpenedDocument *document;

    document = Rockete::getInstance().getCurrentDocument();

    if (document && document->selectedElement) {
        markerList.append(Marker(RocketHelper::getBottomRightPosition(document->selectedElement), 10.0f));
        markerList.append(Marker(RocketHelper::getTopRightPosition(document->selectedElement), 10.0f));
    }

}