#ifndef OPENEDDOCUMENT_H
#define OPENEDDOCUMENT_H

#include "RocketHelper.h"
#include "OpenedFile.h"
#include "StyleSheet.h"
#include "XMLHighlighter.h"
#include <QList>
#include <QTreeWidget>
#include <QTreeWidgetItem>

class OpenedDocument : public OpenedFile
{
public:
    OpenedDocument();

    virtual void initialize();
    void replaceInnerRMLFromTagName(const QString &tag_name, const QString &new_content);
    void regenerateBodyContent();
    virtual void highlightString(const QString &str);
    void addDocumentTextAtCursor(const QString &new_content);
    void addStyleSheetTextAtEnd(const QString &new_content, const QString &file_name);
    QStringList getRCSSFileList();
    void populateHierarchyTreeView(QTreeWidget *tree);

    RMLDocument *rocketDocument;
    Element *selectedElement;
    QList<StyleSheet*> styleSheetList;
    QString highlightedString;

private:
    QStringList getRCSSFileList(Element *element);
    QTreeWidgetItem *getChildrenTree(QTreeWidgetItem *parent, Element *element);

    typedef std::pair< Rocket::Core::String, const Rocket::Core::Property* > NamedProperty;
    typedef std::vector< NamedProperty > NamedPropertyList;
    typedef std::map< Rocket::Core::PseudoClassList, NamedPropertyList > NamedPropertyMap;
};

#endif
