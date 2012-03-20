#include "SnippetsManager.h"

#include <QMimeData>
#include <QDirIterator>
#include <QXmlSimpleReader>
#include "ProjectManager.h"

// Public:

SnippetsManager::SnippetsManager(QWidget *parent)
    : QListWidget(parent),
      QXmlDefaultHandler()
{
}

void SnippetsManager::Initialize()
{
    if(ProjectManager::getInstance().getSnippetsFolderPath().isEmpty())
        return;

    QDirIterator directory_walker(ProjectManager::getInstance().getSnippetsFolderPath(), QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
    QStringList snippetsFileList;

    while(directory_walker.hasNext())
    {
        directory_walker.next();
        if(!directory_walker.fileInfo().isDir() && !directory_walker.fileInfo().isHidden() && directory_walker.fileInfo().suffix()=="snippet")
        {
            snippetsFileList << directory_walker.fileInfo().filePath();
        }
    }


    QXmlSimpleReader xmlReader;
    QXmlInputSource *source;
    bool ok;

    xmlReader.setContentHandler(this);
    xmlReader.setErrorHandler(this);

    foreach(QFile file, snippetsFileList)
    {
        source = new QXmlInputSource(&file);
        ok = xmlReader.parse(source);
        delete(source);
        if(!ok)
        {
            printf("%s could not be parsed as snippet\n", file.fileName());
        }
    }
}

bool SnippetsManager::startDocument()
{
    return true;
}

bool SnippetsManager::startElement(const QString &/*namespaceURI*/, const QString &localName, const QString &/*qName*/, const QXmlAttributes &atts)
{
    currentTag=localName;
    if(currentTag == "Code")
    {
        currentLanguage=(CodeLanguage)atts.value("Index").toInt();
    }
    return true;
}

bool SnippetsManager::endElement(const QString &/*namespaceURI*/, const QString &localName, const QString &/*qName*/)
{
    if(localName == "CodeSnippet")
    {

        QListWidgetItem *item = new QListWidgetItem();

        item->setText(currentTitle);
        item->setToolTip(currentToolTip);
        item->setData(Qt::UserRole, currentCode);
        addItem(item);
    }
    return true;
}

bool SnippetsManager::characters(const QString &ch)
{
    QString usable_string = ch.trimmed();
    usable_string = usable_string.remove("\t");
    if(usable_string.isEmpty())
        return true;


    if(currentTag=="Title")
    {
        currentTitle = usable_string;
    }
    else if(currentTag=="ToolTip")
    {
        currentToolTip = usable_string;
    }
    else if(currentTag=="Code")
    {
        currentCode = usable_string;
    }

    return true;
}

bool SnippetsManager::endDocument()
{
    return true;
}

// public slots:

// protected:

QStringList SnippetsManager::mimeTypes() const
{
    QStringList types;
    types << "text/plain";
    return types;
}

QMimeData *SnippetsManager::mimeData(const QList<QListWidgetItem *> items) const
{
    QString text = "";
    QMimeData *mimeData = new QMimeData();

    foreach(QListWidgetItem *item, items){
        text += item->data(Qt::UserRole).toString();
    }

    mimeData->setText(text);
    return mimeData;
}

// private slots

// private
