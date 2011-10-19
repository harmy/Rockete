#ifndef SETTINGS_H
#define SETTINGS_H

#include <QStringList>
#include <QSettings>
#include <QString>

class Settings
{
public:
    static QStringList getRecentFileList();
    static void setMostRecentFile(const QString &filePath);
    static int getTabSize();

private:
    static QSettings settings;
};

#endif
