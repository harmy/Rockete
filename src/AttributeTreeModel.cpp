#include <QtGui>

#include "AttributeTreeModel.h"
#include "Rockete.h"
#include "ActionManager.h"
#include "ActionSetAttribute.h"
#include "EditionHelper.h"

AttributeTreeModel::AttributeTreeModel(QObject *parent)
: QAbstractItemModel(parent)
{
}

AttributeTreeModel::~AttributeTreeModel()
{
}

QVariant AttributeTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.column() == 2 ) {
        if (index.internalPointer() && index.row() < propertyNameList.size()) {
            if (role == Qt::DecorationRole)
                return QVariant(QIcon(":/images/tools.png"));
        }
        return QVariant();
    }

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    if (index.row() == propertyNameList.size()) {
        if(index.column() == 0)
            return QVariant("Add...");
        else
            return QVariant();
    }

    if (index.column() == 0)
        return QVariant(propertyNameList[index.row()]); 

    if (index.column() == 1)
        return QVariant(propertyValueList[index.row()]); 

    return QVariant("Impossible");
}

bool AttributeTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole || !index.isValid())
        return false;

    if (index.column() == 1) {
        element->SetAttribute(propertyNameList[index.row()].toAscii().data(), value.toByteArray().data());

        ActionManager::getInstance().applyNew(new ActionSetAttribute(document, element,propertyNameList[index.row()],propertyValueList[index.row()],value.toString()));

        propertyValueList[index.row()] = value.toString();
        Rockete::getInstance().repaintRenderingView();
        emit dataChanged(index,index);
    }
    else {
        // :TODO: Check if already exists.
        element->SetAttribute(value.toByteArray().data(), "");
        propertyValueList.push_back("");
        propertyNameList.push_back(value.toByteArray().data());

        emit dataChanged(index,index);
    }

    return true;
}

Qt::ItemFlags AttributeTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    if (index.row() == propertyNameList.size() || index.column() == 1)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant AttributeTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == 0)
            return QVariant("Name");

        if (section == 1)
            return QVariant("Value");
    }

    return QVariant();
}

QModelIndex AttributeTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    if (column == 2 && row < propertyNameList.size()) {
        return createIndex(row,column, EditionHelper::findHelper(propertyNameList[row]));
    }

    return createIndex(row,column,parent.row());
}

QModelIndex AttributeTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid() || index.internalId() == -1)
        return QModelIndex();

    return createIndex(index.internalId(),0,-1);
}

int AttributeTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0 || parent.isValid() || !element)
        return 0;

    return propertyNameList.size()+1;
}

int AttributeTreeModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 3;
}

void AttributeTreeModel::setupData(OpenedDocument *_document, Element *_element)
{
    Rocket::Core::String name, value;
    int index = 0;

    document = _document;
    element = _element;

    propertyNameList.clear();
    propertyValueList.clear();

    if (element) {
        while (element->IterateAttributes(index, name, value)) {
            propertyNameList.push_back(name.CString());
            propertyValueList.push_back(value.CString());
        }
    }
}


