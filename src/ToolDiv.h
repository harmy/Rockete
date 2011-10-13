#ifndef TOOLDIV_H
#define TOOLDIV_H

#include "Tool.h"

class ToolDiv : public Tool
{
public:
    ToolDiv();
    virtual void onElementClicked(Element *element);
    virtual void onRender();
    virtual void onMousePress(const Qt::MouseButton button, const Vector2f &position);

private:
    void processElement(Element *element);
    void insertDiv(Element *element);
};


#endif