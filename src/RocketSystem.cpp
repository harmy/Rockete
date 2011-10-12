#include "RocketSystem.h"

#include <Rocket/Core.h>
#include <QDir>
#include "ToolManager.h"

float RocketSystem::GetElapsedTime()
{
    // :TODO: Compute elapsed time.
    return 1.0f;
}

bool RocketSystem::initialize()
{
    Rocket::Core::SetRenderInterface(&renderInterface);
    Rocket::Core::SetSystemInterface(this );
    Rocket::Core::Initialise();

    // :TODO: Save last screen size.
    return createContext(1024, 768);
}

bool RocketSystem::createContext(const int width, const int height)
{
    if(context)
    {
        context->RemoveReference();
    }

    context = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(width, height));

    if(context == NULL)
    {
        Rocket::Core::Shutdown();
        return false;
    }

    eventListener = new EventListener();

    context->AddEventListener("click", eventListener, true);

    return true;
}

void RocketSystem::loadFonts(const char *directory_path)
{
    QDir directory(directory_path);
    QStringList name_filter_list;
    QStringList file_list;
    QString prefix;

    name_filter_list << "*.otf" << "*.ttf",

    file_list = directory.entryList(name_filter_list);

    prefix = directory_path;
    prefix += "/";

    foreach(const QString &file_name, file_list)
    {
        loadFont(prefix + file_name);
    }
}

void RocketSystem::loadFont(const QString & file)
{
    Rocket::Core::FontDatabase::LoadFontFace(file.toStdString().c_str());
}

void RocketSystem::EventListener::ProcessEvent(Rocket::Core::Event & event) 
{
    ToolManager::getInstance().getCurrentTool()->onElementClicked(event.GetTargetElement());
}
