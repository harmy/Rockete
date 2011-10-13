#ifndef TOOLDIV_H
#define TOOLDIV_H

#include "Tool.h"

class ToolDiv : public Tool
{
public:
    ToolDiv();
    virtual void onElementClicked(Element *element);
    virtual void onRender();

private:
    void processElement(Element *element);
};


#endif