#include "ToolImage.h"

#include "Rockete.h"
#include "ActionManager.h"
#include "ActionInsertElement.h"
#include "ActionSetAttribute.h"
#include <QLabel>
#include <QToolBar>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QFileInfo>
#include "ToolDiv.h"

ToolImage::ToolImage()
: Tool(), currentImageElement(NULL), newImageAction(NULL), itMustPlaceNewImage(false)
{
    QVBoxLayout *layout;
    QToolBar *tool_bar;

    name = "Image tool";
    imageName = ":/images/tool_image.png";
    itAcceptsDrop = true;

    widget = new QWidget();
    layout = new QVBoxLayout();

    layout->addWidget(new QLabel("Insert:"));
    tool_bar = new QToolBar();

    newImageAction = tool_bar->addAction(QIcon(), "New", this, SLOT(insertNew()));
    newImageAction->setCheckable(true);

    layout->addWidget(tool_bar);

    layout->addWidget(new QLabel("Modify:"));
    tool_bar = new QToolBar();
    tool_bar->addAction(QIcon(), "Change source", this, SLOT(changeSource()));

    layout->addWidget(tool_bar);

    widget->setLayout(layout);
}

void ToolImage::onElementClicked(Element *_element)
{
    Element *element = _element;

    while (element) {
        if (element->GetTagName() == "img") {
            currentImageElement = element;
            Rockete::getInstance().selectElement(element);
            return;
        }

        element = element->GetParentNode();
    }

    currentImageElement = NULL;
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

    if (itMustPlaceNewImage) {
        Vector2f size;
        size.x = 100;
        size.y = 100;
        GraphicSystem::drawBox(futureImagePosition - size * 0.5f, size, Color4b(128, 128, 128, 128));
    }
}

void ToolImage::onMousePress(const Qt::MouseButton /*button*/, const Vector2f &/*position*/)
{
    if (itMustPlaceNewImage) {
        Element *element = RocketHelper::getElementUnderMouse();
        if ((element = ToolDiv::getDivParent(element))) {
            QString imageName;
            if (getImageNameFromFileSystem(imageName)) {
                Rocket::Core::XMLAttributes attributes;
                attributes.Set("src", imageName.toAscii().data());
                Element *img = Rocket::Core::Factory::InstanceElement(NULL, "img", "img", attributes);
                insertNew(img, element);
            }
        }
        itMustPlaceNewImage = false;
        newImageAction->setChecked(false);
    }
}

void ToolImage::onMouseRelease(const Qt::MouseButton, const Vector2f &)
{

}

void ToolImage::onMouseMove(const Vector2f &position)
{
    if (itMustPlaceNewImage)
        futureImagePosition = position;
}

void ToolImage::onFileDrop(const QString &url)
{
    QFileInfo fileInfo(url);
    Element *element = RocketHelper::getElementUnderMouse();

    if (element) {
        if (element->GetTagName() == "img") {
            changeSource(element,fileInfo.fileName());
        }
        else {
            element = ToolDiv::getDivParent(element);
            if (element) {
                Rocket::Core::XMLAttributes attributes;
                attributes.Set("src", fileInfo.fileName().toAscii().data());
                Element* img = Rocket::Core::Factory::InstanceElement(NULL, "img", "img", attributes);
                insertNew(img, element);
            }
        }
    }
}

void ToolImage::onUnselect()
{
    currentImageElement = NULL;
}

bool ToolImage::getImageNameFromFileSystem(QString &imageName)
{
    QString fileName = QFileDialog::getOpenFileName(NULL, tr("Select source image..."), "", tr("Image files (*.bmp *.png *.jpg *.jpeg *.tga"));

    if (!fileName.isEmpty()) {
        QFileInfo info(fileName);
        imageName = info.fileName();
        return true;
    }
    else
        return false;
}

// Private slots:

void ToolImage::insertNew()
{
    itMustPlaceNewImage = !itMustPlaceNewImage;
    newImageAction->setChecked(itMustPlaceNewImage);
}

void ToolImage::changeSource()
{
    OpenedDocument *document;
    document = Rockete::getInstance().getCurrentDocument();
    if (document && document->selectedElement) {
        QString imageName;
        if (getImageNameFromFileSystem(imageName)) {
            ActionManager::getInstance().applyNew(new ActionSetAttribute(Rockete::getInstance().getCurrentDocument(), document->selectedElement, "src", imageName));
        }
    }
}

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

void ToolImage::changeSource(Element *element, const QString &imageName)
{
    ActionManager::getInstance().applyNew(new ActionSetAttribute(Rockete::getInstance().getCurrentDocument(), element, "src", imageName));
}

void ToolImage::insertNew(Element *img, Element *element)
{
    ActionManager::getInstance().applyNew(new ActionInsertElement(Rockete::getInstance().getCurrentDocument(), element, img));
    Rockete::getInstance().selectElement(img);
}