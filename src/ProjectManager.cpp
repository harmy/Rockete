#include "ProjectManager.h"

#include <QFile>
#include <QFileInfo>
#include "LocalizationManagerInterface.h"

ProjectManager::ProjectManager()
{
}

ProjectManager::~ProjectManager()
{
}

void ProjectManager::Initialize(const QString &filename)
{
    QFileInfo file_info(filename);
    QFile file(filename);
    domDocument.clear();
    fontPaths.clear();
    texturePaths.clear();
    interfacePaths.clear();

    if (!file.open(QIODevice::ReadOnly))
        return;
    if (!domDocument.setContent(file.readAll())) {
        file.close();
        return;
    }
    file.close();

    projectName = file_info.baseName();

    QDomNodeList node_list = domDocument.elementsByTagName("Font");

    for(int i = 0; i < node_list.count(); i++)
    {
        QDomNode node = node_list.at(i);
        if(node.firstChild().isText())
        {
            fontPaths << node.firstChild().toText().data();
            if(!fontPaths.last().endsWith('/'))
            {
                fontPaths.last().append("/");
            }
        }
    }

    node_list = domDocument.elementsByTagName("Texture");

    for(int i = 0; i < node_list.count(); i++)
    {
        QDomNode node = node_list.at(i);
        if(node.firstChild().isText())
        {
            texturePaths << node.firstChild().toText().data();
            if(!texturePaths.last().endsWith('/'))
            {
                texturePaths.last().append("/");
            }
        }
    }

    node_list = domDocument.elementsByTagName("Interface");

    for(int i = 0; i < node_list.count(); i++)
    {
        QDomNode node = node_list.at(i);
        if(node.firstChild().isText())
        {
            interfacePaths << node.firstChild().toText().data();
            if(!interfacePaths.last().endsWith('/'))
            {
                interfacePaths.last().append("/");
            }
        }
    }

    node_list = domDocument.elementsByTagName("WordList");

    if(!node_list.isEmpty())
    {
        QDomNode node = node_list.at(0);
        if(node.firstChild().isText())
        {
            wordListsPath = node.firstChild().toText().data();
            if(!wordListsPath.endsWith('/'))
            {
                wordListsPath.append("/");
            }
        }
    }

    node_list = domDocument.elementsByTagName("LocalizationFile");

    if(!node_list.isEmpty())
    {
        QDomNode node = node_list.at(0);
        if(node.firstChild().isText())
        {
            if(LocalizationManagerInterface::hasInstance())
            {
                LocalizationManagerInterface::getInstance().initialize(node.firstChild().toText().data());
            }
        }
    }

    node_list = domDocument.elementsByTagName("LocalizationOpeningTag");

    if(!node_list.isEmpty())
    {
        QDomNode node = node_list.at(0);
        if(node.firstChild().isText())
        {
            localizationOpeningTag = node.firstChild().toText().data();
        }
    }

    node_list = domDocument.elementsByTagName("LocalizationClosingTag");

    if(!node_list.isEmpty())
    {
        QDomNode node = node_list.at(0);
        if(node.firstChild().isText())
        {
            localizationClosingTag = node.firstChild().toText().data();
        }
    }

    node_list = domDocument.elementsByTagName("SnippetsFolder");

    if(!node_list.isEmpty())
    {
        QDomNode node = node_list.at(0);
        if(node.firstChild().isText())
        {
            snippetsFolderPath = node.firstChild().toText().data();
            if(!snippetsFolderPath.endsWith('/'))
            {
                snippetsFolderPath.append("/");
            }
        }
    }
    else
    {
        snippetsFolderPath = "snippets/";
    }
}