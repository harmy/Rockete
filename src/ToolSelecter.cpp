#include "ToolSelecter.h"

#include "Rockete.h"

ToolSelecter::ToolSelecter()
: Tool()
{
    name = "Selecter";
    imageName = "images/select.png";
}

void ToolSelecter::onElementClicked(Element *element)
{
    Rockete::getInstance().selectElement(element);
}