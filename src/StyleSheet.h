#ifndef STYLESHEET_H
#define STYLESHEET_H

#include <QString>
#include <QList>
#include <QStringList>

class StyleSheet
{
public:
    StyleSheet();
    ~StyleSheet();

    struct PropertyOwner
    {
        PropertyOwner *ancester;
        QString tagName;
        QString className;
        QString identifier;
        QString special;
    };

    struct Property
    {
        QString name;
        QString value;
    };

    struct PropertyBlock
    {
        QList<PropertyOwner*> ownerList;
        QList<Property*> propertyList;
    };


    bool parse(const QString & content);
    void clear();

private:

    QList<PropertyBlock*> propertyBlockList;

    PropertyOwner *parseOwner(const QString & content);
    Property *parseProperty(const QString & content);
};

#endif
