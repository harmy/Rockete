#ifndef STYLESHEET_H
#define STYLESHEET_H

#include <QString>
#include <QList>
#include <QStringList>
#include "RocketHelper.h"

class StyleSheet
{
public:
    StyleSheet();
    ~StyleSheet();

    struct PropertyOwner{
        PropertyOwner() : ancester(NULL) {};
        ~PropertyOwner() { if(ancester) delete ancester; }
        PropertyOwner *ancester;
        QString tagName;
        QString className;
        QString identifier;
        QString pseudoClassName;
    };

    struct Property{
        QString name;
        QString value;
    };

    struct PropertyBlock{
        QList<PropertyOwner*> ownerList;
        QList<Property*> propertyList;
    };

    bool parse(const QString & content);
    void clear();
    bool findMatchingProperties(QList<PropertyBlock*> & result, const Element *element) const;

private:

    const Element *findDirectInheriter(const PropertyOwner *owner, const Element *element) const;
    PropertyOwner *parseOwner(const QString & content);
    Property *parseProperty(const QString & content);

    QList<PropertyBlock*> propertyBlockList;
};

#endif
