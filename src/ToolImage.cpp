#include "ToolImage.h"

#include "Rockete.h"
#include "ActionManager.h"
#include "ActionInsertElement.h"
#include <QLabel>
#include <QToolBar>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QFileInfo>

ToolImage::ToolImage()
: Tool()
{
    QVBoxLayout *layout;
    QToolBar *tool_bar;

    name = "Image tool";
    imageName = ":/images/tool_image.png";

    widget = new QWidget();
    layout = new QVBoxLayout();

    layout->addWidget(new QLabel("Insert:"));
    tool_bar = new QToolBar();
    tool_bar->addAction(QIcon(), "New", this, SLOT(insertNew()));

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
    OpenedDocument *document;
    Element *element = NULL;

    document = Rockete::getInstance().getCurrentDocument();

    if (document && document->selectedElement)  
        element = document->selectedElement;
    else if(document->rocketDocument)
        element = document->rocketDocument;


    if (element) {
        Element *img;
        QString imageName;

        if (getImageNameFromFileSystem(imageName)) {
            img = new Element("img");
            img->SetAttribute("src", imageName.toStdString().c_str());

            ActionManager::getInstance().applyNew(new ActionInsertElement(Rockete::getInstance().getCurrentDocument(), element, img));
            Rockete::getInstance().selectElement(img);
        }
    }
}

void ToolImage::changeSource()
{
    OpenedDocument *document;
    document = Rockete::getInstance().getCurrentDocument();
    if (document && document->selectedElement) {
        QString imageName;
        if (getImageNameFromFileSystem(imageName)) {
            document->selectedElement->SetAttribute("src", imageName.toStdString().c_str());
            //ActionManager::getInstance().applyNew(new Action(Rockete::getInstance().getCurrentDocument(), element, img));
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