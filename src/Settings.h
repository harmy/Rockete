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
    static QString getProject();
    static void setMostRecentFile(const QString &filePath);
    static void setProject(const QString &filePath);
    static int getTabSize();
    static void setBackroundFileName(const QString &fileName);
    static QString getBackgroundFileName();
    static Rocket::Core::TextureHandle getBackroundTextureHandle();
    static void setFontPath(const QString &dirPath);
    static QString getFontPath();
    static void setTexturePath(const QString &dirPath);
    static QString getTexturePath();
    static void setInterfacePath(const QString &dirPath);
    static QString getInterfacePath();
    static void setWordListsPath(const QString &dirPath);
    static QString getWordListsPath();

private:
    static QSettings settings;
    static Rocket::Core::TextureHandle backgroundTextureHandle;
};

#endif
