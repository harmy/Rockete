#include "Rockete.h"
#include <QtGui/QApplication>
#include "RocketSystem.h"
#include "ToolManager.h"
#include "EditionHelper.h"

int main(int argc, char *argv[])
{
    int result;

    QApplication a( argc, argv );

    if(!RocketSystem::getInstance().initialize())
    {
        return -1;
    }

    Rockete w;
    w.show();
    result = a.exec();

    RocketSystem::getInstance().finalize();

    return result;
}
