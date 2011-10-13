#include "RocketHelper.h"

#include <QtCore>
#include <Rocket/Core.h>
#include "RocketSystem.h"

RMLDocument * RocketHelper::loadDocument(const char * file_path)
{
    return RocketSystem::getInstance().getContext()->LoadDocument( file_path );
}

void RocketHelper::unloadDocument(RMLDocument * rml_document)
{
    RocketSystem::getInstance().getContext()->UnloadDocument(rml_document);
    Rocket::Core::Factory::ClearStyleSheetCache();
}

void RocketHelper::highlightElement(Element * element)
{
    Q_ASSERT(element);

    for (int i = 0; i < element->GetNumBoxes(); i++)
    {
        const Rocket::Core::Box element_box = element->GetBox(i);

        // Content area:
        GraphicSystem::drawBox(element->GetAbsoluteOffset(Rocket::Core::Box::BORDER) + element_box.GetPosition(Rocket::Core::Box::CONTENT), element_box.GetSize(), Color4b(158, 214, 237, 128));

        // Padding area:
        GraphicSystem::drawBox(element->GetAbsoluteOffset(Rocket::Core::Box::BORDER) + element_box.GetPosition(Rocket::Core::Box::PADDING), element_box.GetSize(Rocket::Core::Box::PADDING), element->GetAbsoluteOffset(Rocket::Core::Box::BORDER) + element_box.GetPosition(Rocket::Core::Box::CONTENT), element_box.GetSize(), Color4b(135, 122, 214, 128));

        // Border area:
        GraphicSystem::drawBox(element->GetAbsoluteOffset(Rocket::Core::Box::BORDER) + element_box.GetPosition(Rocket::Core::Box::BORDER), element_box.GetSize(Rocket::Core::Box::BORDER), element->GetAbsoluteOffset(Rocket::Core::Box::BORDER) + element_box.GetPosition(Rocket::Core::Box::PADDING), element_box.GetSize(Rocket::Core::Box::PADDING), Color4b(133, 133, 133, 128));

        // Border area:
        GraphicSystem::drawBox(element->GetAbsoluteOffset(Rocket::Core::Box::BORDER) + element_box.GetPosition(Rocket::Core::Box::MARGIN), element_box.GetSize(Rocket::Core::Box::MARGIN), element->GetAbsoluteOffset(Rocket::Core::Box::BORDER) + element_box.GetPosition(Rocket::Core::Box::BORDER), element_box.GetSize(Rocket::Core::Box::BORDER), Color4b(240, 255, 131, 128));
    }
}

void RocketHelper::drawBoxAroundElement(Element *element, const Color4b &color)
{
    Q_ASSERT(element);

    for (int i = 0; i < element->GetNumBoxes(); i++)
    {
        const Rocket::Core::Box element_box = element->GetBox(i);

        GraphicSystem::drawBox(element->GetAbsoluteOffset(Rocket::Core::Box::BORDER) + element_box.GetPosition(Rocket::Core::Box::CONTENT), element_box.GetSize(), color, false);
    }
}

void RocketHelper::replaceInlinedProperty(Element *element,const QString &property_name, const QString &property_value)
{
    QString properties;
    int property_index;
    int start_index;
    int end_index;

    properties = (element->GetAttribute<Rocket::Core::String>("style","")).CString();

    property_index = properties.indexOf(property_name);
    Q_ASSERT(property_index != -1);
    start_index = properties.indexOf(':',property_index);
    Q_ASSERT(start_index != -1);
    end_index = properties.indexOf(';',start_index);
    Q_ASSERT(end_index != -1);

    properties.replace(start_index + 1,end_index - start_index - 1,property_value);

    element->SetAttribute("style",properties.toStdString().c_str());
}

void RocketHelper::addInlinedProperty(Element *element,const QString &property_name, const QString &property_value)
{
    QString properties;
    QString string_to_append;

    properties = (element->GetAttribute<Rocket::Core::String>("style","")).CString();

    string_to_append = property_name;
    string_to_append.append(":");
    string_to_append.append(property_value);
    string_to_append.append(";");

    properties.append(string_to_append);

    element->SetAttribute("style", properties.toStdString().c_str());
}

void RocketHelper::removeInlinedProperty(Element *element, const QString &property_name)
{
    QString properties;
    int property_index;
    int end_index;

    properties = (element->GetAttribute<Rocket::Core::String>("style","")).CString();

    property_index = properties.indexOf(property_name);
    Q_ASSERT(property_index != -1);
    end_index = properties.indexOf(';',property_index);
    Q_ASSERT(end_index != -1);

    properties.remove(property_index, property_index - end_index - 1);
    element->SetAttribute("style",properties.toStdString().c_str());
}

bool RocketHelper::getInlinedProperty(QString & property_value, Element *element, const QString &property_name)
{
    QString properties;
    int property_index;
    int start_index;
    int end_index;

    properties = (element->GetAttribute<Rocket::Core::String>("style","")).CString();

    property_index = properties.indexOf(property_name);

    if (property_index != -1)
    {
        Q_ASSERT(property_index != -1);
        start_index = properties.indexOf(':', property_index);
        Q_ASSERT(start_index != -1);
        end_index = properties.indexOf(';', start_index);
        Q_ASSERT(end_index != -1);

        property_value = properties.mid(start_index + 1, end_index - start_index - 1);

        return true;
    }

    return false;
}
