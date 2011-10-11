#ifndef ATTRIBUTETREEMODEL_H
#define ATTRIBUTETREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "RocketHelper.h"
#include "OpenedDocument.h"

class AttributeTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    AttributeTreeModel(QObject *parent = 0);
    virtual ~AttributeTreeModel();

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    void setupData(OpenedDocument *_document, Element * _element);

private:

    OpenedDocument *document;
    Element * element;
    QList<QString> propertyNameList;
    QList<QString> propertyValueList;
};

#endif
