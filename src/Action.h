#ifndef ACTION_H
#define ACTION_H

#include <QString>
#include "RocketHelper.h"
#include "OpenedFile.h"
#include "OpenedDocument.h"
#include "OpenedStyleSheet.h"

class Action
{
public:
    enum Type
    {
        ActionUnknown,
        ActionSetProperty,
        ActionSetAttribute
    };

    Action(OpenedDocument *document, Element *element, const QString &variable_name, const QString &current_value, const QString &new_value);

    Type getType() const { return type; };

    void apply();
    void unapply();

private:

    QString variableName;
    QString oldValue;
    QString newValue;
    OpenedFile *targetFile;
    Element *targetElement;
    Type type;
};

#endif
