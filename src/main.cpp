#include "Rockete.h"
#include <QtGui/QApplication>
#include "RocketSystem.h"
#include "ToolManager.h"

int main(int argc, char *argv[])
{
    QApplication a( argc, argv );

    if(RocketSystem::getInstance().initialize())
    {
        RocketSystem::getInstance().loadFonts( "assets" );
    }
    else
    {
        return -1;
    }

    ToolManager::getInstance().initialize();

    Rockete w;
    w.show();
    return a.exec();
}
