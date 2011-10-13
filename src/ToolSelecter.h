#ifndef TOOLSELECTER_H
#define TOOLSELECTER_H

#include "Tool.h"

class ToolSelecter : public Tool
{
public:
    ToolSelecter();
    virtual void onElementClicked(Element *element);
    virtual void onRender();
};


#endif