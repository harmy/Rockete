#include "RocketSystem.h"

#include <Rocket/Core.h>

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

void RocketSystem::loadFonts( const char * directory )
{
    Rocket::Core::String 
        font_names[4];

    // :TODO: Load all existing otf files...

    font_names[0] = "Delicious-Roman.otf";
    font_names[1] = "Delicious-Italic.otf";
    font_names[2] = "Delicious-Bold.otf";
    font_names[3] = "Delicious-BoldItalic.otf";

    for (int i = 0; i < sizeof(font_names) / sizeof(Rocket::Core::String); i++)
    {
        Rocket::Core::String path = Rocket::Core::String( directory ) + "/" + font_names[i];
        loadFont(path.CString());
    }
}

void RocketSystem::loadFont(const QString & file)
{
    Rocket::Core::FontDatabase::LoadFontFace(file.toStdString().c_str());
}

void RocketSystem::EventListener::ProcessEvent(Rocket::Core::Event & event) 
{
    Rockete::getInstance().selectElement(event.GetTargetElement());
}
