#include "Settings.h"

#include <QSettings>
#include <QStringList>

#define APPNAME Rockete


QStringList Settings::getRecentFileList()
{
    return settings.value("File/Recents").value< QStringList >();
}

void Settings::setMostRecentFile(const QString &filePath)
{
    QStringList currentList;

    currentList = getRecentFileList();

    foreach(const QString &item, currentList) {
        if (item==filePath) {
            currentList.removeOne(item);
            break;
        }
    }

    currentList.push_front(filePath);

    settings.setValue("File/Recents", currentList);
}

int Settings::getTabSize()
{
    // :TODO: Make a setting ;)
    return 4;
}

QSettings Settings::settings("FishingCactus", "Rockete");
