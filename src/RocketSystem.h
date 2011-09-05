#ifndef ROCKETSYSTEM_H
#define ROCKETSYSTEM_H

#include "RocketRenderInterface.h"
#include <Rocket/Core/SystemInterface.h>
#include "Rockete.h"

class RocketSystem : public Rocket::Core::SystemInterface
{
public:

    typedef Rocket::Core::Context Context;

    bool initialize();
    bool createContext(const int width, const int height);
    void loadFonts(const char * directory);

    Context * getContext() 
    {
        return context;
    }

    static RocketSystem & getInstance()
    {
        static RocketSystem
            instance;

        return instance;
    }

    virtual float GetElapsedTime();

private:

    class EventListener : public Rocket::Core::EventListener
    {
    public:
        EventListener() {
        }

        virtual void ProcessEvent(Rocket::Core::Event & event);

        virtual void OnDetach(Rocket::Core::Element *) {
            delete this;
        }
    };

    RocketRenderInterface renderInterface;
    Context * context;
    EventListener * eventListener;

};

#endif
