#include "RocketSystem.h"

#include <Rocket/Core.h>
#include <Rocket/Core/FreeType/FontProvider.h>
#include <Rocket/Controls.h>
#include <QDir>
#include "ToolManager.h"
#include "LocalizationManagerInterface.h"
#include "ProjectManager.h"
#include "RocketFileInterface.h"

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

int RocketSystem::TranslateString(Rocket::Core::String& translated, const Rocket::Core::String& input)
{
    QString
        translated_string,
        input_string;
    
    if(!LocalizationManagerInterface::hasInstance())
    {
        translated = input;
        return 0;
    }


    input_string = input.CString();

    if(input_string.contains(ProjectManager::getInstance().getLocalizationOpeningTag()))
    {
        QString localization_identifier;
        int starting_index = input_string.indexOf(ProjectManager::getInstance().getLocalizationOpeningTag()) + ProjectManager::getInstance().getLocalizationOpeningTag().size();
        
        
        int identifier_size = input_string.indexOf(ProjectManager::getInstance().getLocalizationClosingTag(),starting_index) - starting_index;

        localization_identifier = input_string.mid(starting_index, identifier_size);
        
        translated_string = LocalizationManagerInterface::getInstance().getLocalizationForIdentifier(localization_identifier.trimmed());
        
        translated_string = input_string.replace(ProjectManager::getInstance().getLocalizationOpeningTag()+localization_identifier+ProjectManager::getInstance().getLocalizationClosingTag(), translated_string);

        if(translated_string.isEmpty())
        {
            printf("warning: could not find localization identifier \"%s\"\n", localization_identifier.toAscii().data() );
            translated = input;
            return 0;
        }
        else
        {
            translated = translated_string.toUtf8();
            return 1;
        }
    }
    else
    {
        translated = input;
        return 0;
    }

    
}

bool RocketSystem::initialize()
{
    Rocket::Core::SetRenderInterface(&renderInterface);
    Rocket::Core::SetSystemInterface(this);
    Rocket::Core::SetFileInterface(new RocketFileInterface());
    Rocket::Core::Initialise();

    Rocket::Core::FreeType::FontProvider::Initialise();
    Rocket::Controls::Initialise();

    // :TODO: Save last screen size.
    return createContext(1024, 768);
}

void RocketSystem::finalize()
{
    //Rocket::Core::FreeType::FontProvider::Shutdown();
    Rocket::Core::Shutdown();
    Rocket::Core::SetRenderInterface(0);
    Rocket::Core::SetSystemInterface(0);
    Rocket::Core::SetFileInterface(0);
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
