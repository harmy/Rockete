#include "ToolImage.h"

#include "Rockete.h"

ToolImage::ToolImage()
: Tool()
{
    name = "Image tool";
    imageName = ":/images/tool_image.png";
}

void ToolImage::onElementClicked(Element *_element)
{
    Element *element = _element;

    while (element) {
        if (element->GetTagName() == "img") {
            Rockete::getInstance().selectElement(element);
            break;
        }

        element = element->GetParentNode();
    }
}

void ToolImage::onRender()
{
    OpenedDocument *document;

    document = Rockete::getInstance().getCurrentDocument();

    if (document) {
        glLineWidth(2.0f);
        processElement(document->rocketDocument);

        if(document->selectedElement) {
            RocketHelper::drawBoxAroundElement(document->selectedElement, Color4b(10, 200, 240, 255));
        }

        glLineWidth(1.0f);
    }
}

void ToolImage::onMousePress(const Qt::MouseButton /*button*/, const Vector2f &/*position*/)
{

}

void ToolImage::onMouseRelease(const Qt::MouseButton, const Vector2f &)
{

}

void ToolImage::onMouseMove(const Vector2f &/*position*/)
{

}

// Private slots:


// Private:

void ToolImage::processElement(Element *element)
{
    if (element->GetTagName() == "img") {
        RocketHelper::drawBoxAroundElement(element, Color4b(10, 200, 128, 255));
    }

    for (int child_index=0; child_index < element->GetNumChildren(); ++child_index) {
        processElement(element->GetChild(child_index));
    }
}