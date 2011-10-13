#include "ToolDiv.h"

#include "Rockete.h"
#include "RocketHelper.h"
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

void ToolDiv::onElementClicked(Element *element)
{

}

void ToolDiv::onRender()
{
    RMLDocument *document;

    document = Rockete::getInstance().getCurrentDocument()->rocketDocument;

    processElement(document);
}

void ToolDiv::processElement(Element *element)
{
    if(element->GetTagName() == "div")
    {
        RocketHelper::highlightElement(element);
    }

    for(int child_index=0; child_index < element->GetNumChildren(); ++child_index)
    {
        processElement( element->GetChild(child_index));
    }
}
