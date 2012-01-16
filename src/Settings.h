#ifndef SETTINGS_H
#define SETTINGS_H

#include <QStringList>
#include <QSettings>
#include <QString>
#include <Rocket/Core.h>

class Settings
{
public:
    static QStringList getRecentFileList();
    static void setMostRecentFile(const QString &filePath);
    static int getTabSize();
    static void setBackroundFileName(const QString &fileName);
    static QString getBackgroundFileName();
    static Rocket::Core::TextureHandle getBackroundTextureHandle();

private:
    static QSettings settings;
    static Rocket::Core::TextureHandle backgroundTextureHandle;
};

#endif
