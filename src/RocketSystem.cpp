#include "RocketSystem.h"

#include <Rocket/Core.h>
#include <Rocket/Core/FreeType/FontProvider.h>
#include <QDir>
#include "ToolManager.h"

RocketSystem::RocketSystem() :
    renderInterface(),
    context( 0 ),
    eventListener( 0 )
{

}

RocketSystem::~RocketSystem()
{
    Rocket::Core::Shutdown();
}

float RocketSystem::GetElapsedTime()
{
    // :TODO: Compute elapsed time.
    return 1.0f;
}

bool RocketSystem::initialize()
{
    Rocket::Core::SetRenderInterface(&renderInterface);
    Rocket::Core::SetSystemInterface(this);
    Rocket::Core::Initialise();

    Rocket::Core::FreeType::FontProvider::Initialise();

    // :TODO: Save last screen size.
    return createContext(1024, 768);
}

void RocketSystem::finalize()
{
    Rocket::Core::FreeType::FontProvider::Shutdown();
    Rocket::Core::Shutdown();
    Rocket::Core::SetRenderInterface( 0 );
    Rocket::Core::SetSystemInterface( 0 );
}

bool RocketSystem::createContext(const int width, const int height)
{
    if (context)
        context->RemoveReference();

    context = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(width, height));

    if (context == NULL) 
    {
        Rocket::Core::Shutdown();
        return false;
    }

    eventListener = new EventListener();

    context->AddEventListener("click", eventListener, true);

    return true;
}

void RocketSystem::loadFonts(const QString &directory_path)
{
    QDir directory(directory_path);
    QStringList name_filter_list;
    QStringList file_list;
    QString prefix;

    name_filter_list << "*.otf" << "*.ttf";

    file_list = directory.entryList(name_filter_list);

    foreach(const QString &file_name, file_list) {
        loadFont(directory_path + file_name);
    }
}

void RocketSystem::loadFont(const QString &file)
{
    Rocket::Core::String
        r_string = file.toAscii().data();

    Rocket::Core::FreeType::FontProvider::LoadFontFace(r_string);
}

void RocketSystem::EventListener::ProcessEvent(Rocket::Core::Event &event)
{
    ToolManager::getInstance().getCurrentTool()->onElementClicked(event.GetTargetElement());
}


RocketSystem* RocketSystem::instance = 0;
