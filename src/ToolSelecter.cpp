#include "ToolSelecter.h"

#include "Rockete.h"
#include <QLabel>
#include <QGridLayout>

ToolSelecter::ToolSelecter()
: Tool()
{
    name = "Selecter";
    imageName = "images/tool_select.png";
    layout = new QGridLayout();
    layout->addWidget(new QLabel("Selecter"));
}

void ToolSelecter::onElementClicked(Element *element)
{
    Rockete::getInstance().selectElement(element);
}