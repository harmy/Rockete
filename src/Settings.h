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

private:
    static QSettings settings;
};

#endif
