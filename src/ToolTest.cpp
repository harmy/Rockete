#include "ToolTest.h"

#include "Rockete.h"
#include "RocketHelper.h"
#include "OpenedDocument.h"
#include "ActionManager.h"
#include "ActionSetInlineProperty.h"
#include "ActionInsertElement.h"
#include "ActionGroup.h"
#include <QLabel>
#include <QToolBar>
#include <QVBoxLayout>

#define MARKER_INDEX_BOTTOM_RIGHT 0
#define MARKER_INDEX_TOP_RIGHT 1

ToolTest::ToolTest()
: Tool()
{
    name = "Test tool";
    imageName = ":/images/tool_test.png";
}

void ToolTest::onElementClicked(Element *_element)
{
    if (_element) {

        Element *parent;
        Rocket::Core::String name, value;

        parent = _element;
        bool hasOnClick = false;

        do
        {
            int index = 0;
            while(parent->IterateAttributes<Rocket::Core::String>(index, name, value))
            {
                if(name=="onclick")
                {
                    hasOnClick = true;
                }
            }

            if(!hasOnClick)
            {
                parent = parent->GetParentNode();
            }
        } while (!hasOnClick && parent != NULL);

        return;
    }
}

// Private slots:

// Private: