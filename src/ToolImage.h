#ifndef TOOLIMAGE_H
#define TOOLIMAGE_H

#include "Tool.h"
#include <QObject>
#include <QString>

class ToolImage : public Tool
{
    Q_OBJECT

public:
    ToolImage();
    virtual void onElementClicked(Element *element);
    virtual void onRender();
    virtual void onMousePress(const Qt::MouseButton button, const Vector2f &position);
    virtual void onMouseRelease(const Qt::MouseButton button, const Vector2f &position);
    virtual void onMouseMove(const Vector2f &position);
    virtual void onFileDrop(const QString &url);
    virtual void onUnselect();
    static bool getImageNameFromFileSystem(QString &imageName);

private slots:
    void insertNew();
    void changeSource();

private:
    void processElement(Element *element);
    void changeSource(Element *element, const QString &new_source);
    void insertNew(Element *img, Element *element);
    Element *currentImageElement;
    QAction *newImageAction;
    bool itMustPlaceNewImage;
    Vector2f futureImagePosition;
};


#endif