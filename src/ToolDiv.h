#ifndef TOOLDIV_H
#define TOOLDIV_H

#include "Tool.h"
#include <QObject>

class ToolDiv : public Tool
{
    Q_OBJECT

public:
    ToolDiv();
    virtual void onElementClicked(Element *element);
    virtual void onRender();
    virtual void onMousePress(const Qt::MouseButton button, const Vector2f &position);
    virtual void onMouseRelease(const Qt::MouseButton button, const Vector2f &position);
    virtual void onMouseMove(const Vector2f &position);
    virtual void onUnselect();


private slots:
    void insertDiv();
    void expandWidth();
    void setInlineBlockDisplay();
    void setLeftAlignment();
    void setRightAlignment();

private:
    void processElement(Element *element);
    void insertDiv(Element *element);
    void setupMarkers();
    Element *selectedElement;
    Vector2f initialPosition;
    bool itIsResizing;
    int currentMarkerIndex;

};


#endif