#include "ToolDiv.h"

#include "Rockete.h"
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
    Rockete::getInstance().selectElement(element);
}