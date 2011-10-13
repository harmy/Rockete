#ifndef ROCKETHELPER_H
#define ROCKETHELPER_H

#include <Rocket/Core.h>
#include <QString>

typedef Rocket::Core::Element Element;
typedef Rocket::Core::ElementDocument RMLDocument;
typedef Rocket::Core::Vector2f Vector2f;
typedef Rocket::Core::Colourb Color4b;

class RocketHelper
{
public:
    static RMLDocument * loadDocument(const char *file_path);
    static void unloadDocument(RMLDocument *rml_document);
    static void highlightElement(Element *element);
    static void drawBoxAroundElement(Element *element, const Color4b &color);

    static void replaceInlinedProperty(Element *element,const QString &property_name, const QString &property_value);
};

#endif
