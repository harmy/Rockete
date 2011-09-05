#ifndef PROPERTYTREEMODEL_H
#define PROPERTYTREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "RocketHelper.h"

class PropertyTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    PropertyTreeModel(QObject *parent = 0);
    ~PropertyTreeModel();

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    void setupData(Element * _element);

private:
    typedef std::pair< Rocket::Core::String, const Rocket::Core::Property* > NamedProperty;
    typedef std::vector< NamedProperty > NamedPropertyList;
    typedef std::map< Rocket::Core::PseudoClassList, NamedPropertyList > NamedPropertyMap;
    struct Property {
        QString name;
        QString value;
        int sourceLineNumber;
    };
    struct PropertySet {
        QString displayedName;
        QString sourceFile;
        QString baseElement;
        QList<QString> pseudoClassList;
        int sourceLineNumber;
        bool itIsInherited;
        QList<Property*> propertyList;
    };
    void clearData();
    void buildElementProperties(Rocket::Core::Element* element, Rocket::Core::Element* primary_element);
    void buildProperties(const NamedPropertyList& properties);
    void buildProperty(const Rocket::Core::String& name, const Rocket::Core::Property* property);

    Element * currentElement;
    QList<PropertySet*> propertySetList;
    PropertySet * currentPropertySet;


};

#endif
