#include <QtGui>

#include "PropertyTreeModel.h"
#include "Rockete.h"
#include "OpenedFile.h"
#ifndef QT_NO_DEBUG
#include "modeltest/modeltest.h"
#endif

#define PROPERTY_SET_ID -1

PropertyTreeModel::PropertyTreeModel(QObject *parent)
: QAbstractItemModel(parent)
{
#ifndef QT_NO_DEBUG
    new ModelTest(this);
#endif
}

PropertyTreeModel::~PropertyTreeModel()
{
    clearData();
}

QVariant PropertyTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    if(index.internalId() == PROPERTY_SET_ID)
    {
        PropertySet * property_set = propertySetList[index.row()];
        if(index.column() == 0)
        {
            if(property_set->itIsInherited)
            {
                return QVariant("inherit");
            }
            else
            {
                return QVariant(property_set->displayedName);
            }
        }
        else
        {
            return QVariant(property_set->sourceFile);
        }
    }
    else
    {
        if(index.column() == 0)
        {
            return QVariant(propertySetList[index.internalId()]->propertyList[index.row()]->name);
        }
        else
        {
            return QVariant(propertySetList[index.internalId()]->propertyList[index.row()]->value);
        }
    }

    return QVariant("Impossible");
}

bool PropertyTreeModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if(role != Qt::EditRole || !index.isValid())
        return false;

    if(index.column() == 1)
    {
        const PropertySet * property_set = propertySetList[index.internalId()];
        Property * property =  property_set->propertyList[index.row()];

        OpenedFile *opened_file = Rockete::getInstance().getOpenedFile(property_set->sourceFile.toStdString().c_str(), true);

        if(opened_file)
        {
            int source_line_number;
            QString new_line;

            source_line_number = opened_file->findLineNumber(property->name,property->sourceLineNumber);
            new_line = "    " + property->name + ": " + value.toString() + ";";

            opened_file->replaceLine(source_line_number, new_line);

            property->value = value.toString();

            opened_file->save();
            Rockete::getInstance().reloadCurrentDocument();

            emit dataChanged(index,index);

            return true;
        }
    }

    return false;
}

Qt::ItemFlags PropertyTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid() || index.internalId() == PROPERTY_SET_ID)
        return 0;

    if( index.column() == 1 )
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant PropertyTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if( section == 0 )
            return QVariant("Name");

        if( section == 1 )
            return QVariant("Value");
    }

    return QVariant();
}

QModelIndex PropertyTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent) || propertySetList.isEmpty())
        return QModelIndex();

    if(!parent.isValid())
        return createIndex(row,column,PROPERTY_SET_ID);

    return createIndex(row,column,parent.row());
}

QModelIndex PropertyTreeModel::parent(const QModelIndex &index) const
{
    if(!index.isValid() || index.internalId() == PROPERTY_SET_ID)
        return QModelIndex();

    return createIndex(index.internalId(),0,PROPERTY_SET_ID);
}

int PropertyTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0 || propertySetList.isEmpty())
        return 0;

    if(!parent.isValid())
        return propertySetList.size();

    if(parent.internalId() == PROPERTY_SET_ID)
    {
        return propertySetList[parent.row()]->propertyList.size();
    }

    return 0;
}

int PropertyTreeModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 2;
}

void PropertyTreeModel::setupData(Element * _element)
{
    clearData();
    currentElement = _element;
    if(currentElement)
    {
        currentPropertySet = NULL;
        buildElementProperties(_element, _element);
    }
}

// Private:

void PropertyTreeModel::clearData()
{
    for(int i=0;i<propertySetList.size();++i)
    {
        for(int j=0;j<propertySetList[i]->propertyList.size();++j)
        {
            delete propertySetList[i]->propertyList[j];
        }

        delete propertySetList[i];
    }

    propertySetList.clear();
}

void PropertyTreeModel::buildElementProperties(Rocket::Core::Element* element, Rocket::Core::Element* primary_element)
{
    NamedPropertyMap property_map;

    int property_index = 0;
    Rocket::Core::String property_name;
    Rocket::Core::PseudoClassList property_pseudo_classes;
    const Rocket::Core::Property* property;

    while (element->IterateProperties(property_index, property_pseudo_classes, property_name, property))
    {
        // Check that this property isn't overridden or just not inherited.
        if (primary_element->GetProperty(property_name) != property)
            continue;

        NamedPropertyMap::iterator i = property_map.find(property_pseudo_classes);
        if (i == property_map.end())
            property_map[property_pseudo_classes] = NamedPropertyList(1, NamedProperty(property_name, property));
        else
        {
            // Find a place in this list of properties to insert the new one.
            NamedPropertyList& properties = (*i).second;
            NamedPropertyList::iterator insert_iterator = properties.begin();
            while (insert_iterator != properties.end())
            {
                int source_cmp = strcasecmp((*insert_iterator).second->source.CString(), property->source.CString());
                if (source_cmp > 0 ||
                    (source_cmp == 0 && (*insert_iterator).second->source_line_number >= property->source_line_number))
                    break;

                ++insert_iterator;
            }

            (*i).second.insert(insert_iterator, NamedProperty(property_name, property));
        }
    }

    if (!property_map.empty())
    {
        // Inherited?
        if( currentPropertySet )
        {
            if (element != primary_element)
            {
                currentPropertySet->itIsInherited = true;
                currentPropertySet->baseElement = element->GetTagName().CString();
            }
            else
            {
                currentPropertySet->itIsInherited = false;
            }
        }

        NamedPropertyMap::iterator base_properties = property_map.find(Rocket::Core::PseudoClassList());
        if (base_properties != property_map.end())
        {
            currentPropertySet = new PropertySet();
            propertySetList.push_back(currentPropertySet);
            buildProperties((*base_properties).second);
        }

        for (NamedPropertyMap::iterator i = property_map.begin(); i != property_map.end(); ++i)
        {
            currentPropertySet = new PropertySet();
            propertySetList.push_back(currentPropertySet);

            // Skip the base property list, we've already printed it.
            if (i == base_properties)
                continue;

            // Pseudo-class list.
            for (Rocket::Core::PseudoClassList::const_iterator j = (*i).first.begin(); j != (*i).first.end(); ++j)
            {
                currentPropertySet->pseudoClassList.push_back((*j).CString());
            }

            buildProperties((*i).second);
        }
    }

    if (element->GetParentNode() != NULL)
        buildElementProperties(element->GetParentNode(), primary_element);
}

void PropertyTreeModel::buildProperties(const NamedPropertyList& properties)
{
    Rocket::Core::String last_source;
    int last_source_line = -1;

    for (size_t i = 0; i < properties.size(); ++i)
    {
        if (i == 0 ||
            last_source != properties[i].second->source ||
            last_source_line != properties[i].second->source_line_number)
        {
            last_source = properties[i].second->source;
            last_source_line = properties[i].second->source_line_number;

            // Inline or from source?
            if (last_source.Empty() && last_source_line == 0)
            {
                currentPropertySet->displayedName = "inline";
                currentPropertySet->sourceFile = "inline";
                currentPropertySet->sourceLineNumber = -1;
            }
            else
            {
                OpenedFile *opened_file;

                opened_file = Rockete::getInstance().getOpenedFile(last_source.CString(),true);

                currentPropertySet->sourceFile = QString(last_source.CString()).trimmed();
                currentPropertySet->sourceLineNumber = last_source_line;

                if(currentPropertySet->sourceFile.endsWith(".rml")) // Compute offset in RML file.
                {
                    currentPropertySet->sourceLineNumber += opened_file->findLineNumber("<style>");
                }

                if(opened_file)
                {
                    QString line_content = opened_file->getLine(currentPropertySet->sourceLineNumber);

                    line_content = line_content.trimmed();

                    if(line_content == "{")
                    {
                        line_content = opened_file->getLine(currentPropertySet->sourceLineNumber-1).trimmed();
                    }

                    currentPropertySet->displayedName = line_content;
                }
                else
                {
                    currentPropertySet->displayedName = currentPropertySet->sourceFile;
                }
            }
        }

        buildProperty(properties[i].first, properties[i].second);
    }
}

void PropertyTreeModel::buildProperty(const Rocket::Core::String& name, const Rocket::Core::Property* property)
{
    Property * new_property = new Property;

    new_property->name = name.CString();
    new_property->value = property->ToString().CString();
    new_property->sourceLineNumber = currentPropertySet->sourceLineNumber;

    if(currentPropertySet->sourceLineNumber != -1)
    {
        Q_ASSERT(currentPropertySet->sourceFile == property->source.CString());
    }

    currentPropertySet->propertyList.push_back(new_property);
}
