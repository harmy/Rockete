#include "ToolDiv.h"

#include "Rockete.h"
#include "RocketHelper.h"
#include "OpenedDocument.h"
#include "ActionManager.h"
#include <QLabel>
#include <QGridLayout>

ToolDiv::ToolDiv()
: Tool()
{
    QLayout *layout;

    name = "Div tool";
    imageName = "images/tool_div.png";
    layout = new QGridLayout();
    layout->addWidget(new QLabel("Div tool"));
    widget = new QWidget();
    widget->setLayout(layout);
}

void ToolDiv::onElementClicked(Element *_element)
{
    Element *element = _element;

    while(element)
    {
        if(element->GetTagName() == "div")
        {
            Rockete::getInstance().selectElement(element);
            break;
        }

        element = element->GetParentNode();
    }
}

void ToolDiv::onRender()
{
    OpenedDocument *document;

    document = Rockete::getInstance().getCurrentDocument();

    if(document)
    {
        glLineWidth(2.0f);
        processElement(document->rocketDocument);

        if(document->selectedElement)
        {
            RocketHelper::drawBoxAroundElement(document->selectedElement, Color4b(10, 10, 240, 255));
        }

        glLineWidth(1.0f);
    }
}

void ToolDiv::onMousePress(const Qt::MouseButton button, const Vector2f &/*position*/)
{
    if(button == Qt::RightButton)
    {
        insertDiv(Rockete::getInstance().getCurrentDocument()->selectedElement);
    }
}

// Private:

void ToolDiv::processElement(Element *element)
{
    if(element->GetTagName() == "div")
    {
        RocketHelper::drawBoxAroundElement(element, Color4b(10, 240, 10, 255));
    }

    for(int child_index=0; child_index < element->GetNumChildren(); ++child_index)
    {
        processElement( element->GetChild(child_index));
    }
}

void ToolDiv::insertDiv(Element *element)
{
    Element *div;

    div = new Element("div");
    div->SetAttribute("style", "width:85%; height:66px;");

    ActionManager::getInstance().applyNew(new Action(Rockete::getInstance().getCurrentDocument(), element, div));
    Rockete::getInstance().selectElement(div);
}