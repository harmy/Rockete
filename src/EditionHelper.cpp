#include "EditionHelper.h"

#include "EditionHelperColor.h"

EditionHelper::EditionHelper()
{
}

EditionHelper::~EditionHelper()
{
}

void EditionHelper::initialize()
{
    helperList.append(new EditionHelperColor);
}

EditionHelper *EditionHelper::findHelper(const QString &propertyName)
{
    if (propertyName.contains("color")) {
        return helperList[0];
    }

    return NULL;
}

QList<EditionHelper *> EditionHelper::helperList;