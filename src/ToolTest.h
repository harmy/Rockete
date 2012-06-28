#ifndef TOOLTEST_H
#define TOOLTEST_H

#include "Tool.h"
#include <QObject>

class ToolTest : public Tool
{
    Q_OBJECT

public:
    ToolTest();
    virtual void onElementClicked(Element *element);

private slots:

private:

};


#endif