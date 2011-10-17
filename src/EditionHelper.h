#ifndef EDITIONHELPER_H
#define EDITIONHELPER_H

#include <QList>
#include <QString>

class EditionHelper
{
public:
    EditionHelper();
    virtual ~EditionHelper();
    virtual bool help(QString &result) = 0;
    static void initialize();
    static EditionHelper *findHelper(const QString &propertyName);

private:
    static QList<EditionHelper *> helperList;
};
#endif