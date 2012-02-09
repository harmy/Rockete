#include "Rockete.h"

#include <QFileDialog>
#include <QTextEdit>
#include <QFileInfo>
#include <QInputDialog>
#include <QTextDocument>
#include <QGridLayout>
#include <QColorDialog>
#include <QComboBox>
#include <QApplication>
#include <QMessageBox>
#include "AttributeTreeModel.h"
#include "RocketSystem.h"
#include "ActionManager.h"
#include "ToolManager.h"
#include "EditionHelper.h"
#include "Settings.h"
#include <QDirIterator>

struct LocalScreenSizeItem
{
    LocalScreenSizeItem(const int _width, const int _height, const char *text=NULL)
    {
        width = _width;
        height = _height;
        displayedString = QString::number(width) + "x" + QString::number(height);

        if (text)
        {
            displayedString += " (";
            displayedString += text;
            displayedString += ")";
        }
    }

    QString displayedString;
    int width;
    int height;
};

Rockete::Rockete(QWidget *parent, Qt::WFlags flags)
: QMainWindow(parent, flags), currentDocument(NULL), isReloadingFile(false)
{
    instance = this;

    ui.setupUi(this);

    ui.currentToolTab->setLayout(new QGridLayout());

    connect(ui.menuRecent, SIGNAL(triggered(QAction*)), this, SLOT(menuRecentFileClicked(QAction*)));
    generateMenuRecent();

    renderingView = ui.renderingView;

    // Toolbar.
    ui.mainToolBar->addAction(ui.actionOpen);
    ui.mainToolBar->addAction(ui.actionSave);
    ui.mainToolBar->addSeparator();
    ui.mainToolBar->addAction(ui.actionUndo);
    ui.mainToolBar->addAction(ui.actionRedo);
    ui.mainToolBar->addSeparator();
    ui.mainToolBar->addAction(ui.actionReload);
    ui.mainToolBar->addSeparator();
    ui.mainToolBar->addAction(ui.actionZoom_in);
    ui.mainToolBar->addAction(ui.actionZoom_out);

    ui.mainToolBar->addSeparator();

    if( QApplication::arguments().count() > 1 )
    {
        openProject(QApplication::arguments()[1].toAscii().data());
    }
    else if(!Settings::getProject().isEmpty())
    {
        openProject(Settings::getProject());
    }

    ToolManager::getInstance().initialize();
    EditionHelper::initialize();

    ToolManager::getInstance().setup(ui.mainToolBar, ui.menuTools);

    attributeTreeModel = new AttributeTreeModel();
    propertyTreeModel = new PropertyTreeModel();

    ui.mainToolBar->addSeparator();

    searchBox = new QComboBox(this);
    searchBox->setEditable(true);
    searchBox->setInsertPolicy( QComboBox::InsertAtTop );
    QAction *action = new QAction(QIcon(":/images/search.png"), "search", searchBox);

    QString shortcut_string = "F" + QString::number(4);
    action->setShortcut(QKeySequence(shortcut_string));

    connect( searchBox, SIGNAL(activated(const QString&)), (QObject*)this, SLOT(searchBoxActivated()));
    connect( action, SIGNAL(triggered()), (QObject*)this, SLOT(searchBoxActivated()));
    ui.mainToolBar->addWidget(searchBox);
    ui.mainToolBar->addAction(action);

    fileWatcher = new QFileSystemWatcher();

    connect(fileWatcher, SIGNAL(fileChanged(const QString &)), (QObject*)this, SLOT(fileHasChanged(const QString &)));
}

Rockete::~Rockete()
{
    delete attributeTreeModel;
    delete propertyTreeModel;
    clear();
}

void Rockete::repaintRenderingView()
{
    renderingView->repaint();
}

void Rockete::fillAttributeView()
{
    attributeTreeModel->setupData(currentDocument, currentDocument->selectedElement);
    ui.attributeTreeView->reset();
    ui.attributeTreeView->setModel(attributeTreeModel);
    ui.attributeTreeView->header()->setResizeMode(0, QHeaderView::ResizeToContents);
    ui.attributeTreeView->header()->setResizeMode(1, QHeaderView::Stretch);
    ui.attributeTreeView->header()->setResizeMode(2, QHeaderView::ResizeToContents);
    ui.attributeTreeView->header()->setStretchLastSection(false);
}

void Rockete::fillPropertyView()
{
    propertyTreeModel->setupData(currentDocument, currentDocument->selectedElement);
    ui.propertyTreeView->reset();
    ui.propertyTreeView->setModel(propertyTreeModel);
    ui.propertyTreeView->expandAll();
    ui.propertyTreeView->header()->setResizeMode(0, QHeaderView::ResizeToContents);
    ui.propertyTreeView->header()->setResizeMode(1, QHeaderView::Stretch);
    //ui.propertyTreeView->header()->setResizeMode(2, QHeaderView::ResizeToContents);
    //ui.propertyTreeView->header()->setStretchLastSection(false);
}

void Rockete::selectElement(Element *element)
{
    if(element != currentDocument->selectedElement)
    {
        currentDocument->selectedElement = element;
        repaintRenderingView();
        fillAttributeView();
        fillPropertyView();
    }
}

void Rockete::clear()
{
    for (int i=0;i<openedFileList.size();++i)
    {
        delete openedFileList[i];
    }

    openedFileList.clear();
    documentList.clear();
    styleSheetList.clear();

    // :TODO: Unload current doc, etc...
}

void Rockete::reloadCurrentDocument()
{
    if (currentDocument)
    {
        // :TODO: Clean this part.
        renderingView->changeCurrentDocument(NULL);
        currentDocument->selectedElement = NULL;
        if(currentDocument->rocketDocument)
            RocketHelper::unloadDocument(currentDocument->rocketDocument);

        currentDocument->rocketDocument = RocketHelper::loadDocument(currentDocument->fileInfo.filePath().toAscii().data());
        currentDocument->rocketDocument->RemoveReference();
        renderingView->changeCurrentDocument(currentDocument);
    }
}

int Rockete::getTabIndexFromFileName(const char * name)
{
    for (int i = 0; i < ui.codeTabWidget->count(); ++i)
    {
        if (ui.codeTabWidget->tabText(i) == name)
        {
            return i;
        }
    }

    return -1;
}

OpenedDocument *Rockete::getDocumentFromFileName(const char * name)
{
    for (int i = 0; i < documentList.size(); ++i)
    {
        if (documentList[i]->fileInfo.fileName() == name)
        {
            return documentList[i];
        }
    }

    return NULL;
}

OpenedStyleSheet *Rockete::getStyleSheetFromFileName(const char * name)
{
    for (int i = 0; i < styleSheetList.size(); ++i)
    {
        if (styleSheetList[i]->fileInfo.fileName() == name)
        {
            return styleSheetList[i];
        }
    }

    return NULL;
}

OpenedFile *Rockete::getOpenedFile(const char * file_path, const bool try_to_open)
{
    QFileInfo file_info(file_path);

    for (int i = 0; i < openedFileList.size(); ++i)
    {
        if (openedFileList[i]->fileInfo.fileName() == file_info.fileName())
        {
            return openedFileList[i];
        }
    }

    if (try_to_open)
    {
        openFile(file_info.filePath());
        return getOpenedFile(file_path);
    }

    return NULL;
}

// Public slots:

void Rockete::menuOpenClicked()
{
    QString file_path = QFileDialog::getOpenFileName(this, tr("Open libRocket file..."), "", tr("libRocket files (*.rml *.rcss);;Rockete project(*.rproj)"));

    if (!file_path.isEmpty())
    {
        openFile(file_path);
    }
}

void Rockete::menuOpenProjectClicked()
{
    QString file_path = QFileDialog::getOpenFileName(this, tr("Open Rockete project..."), "", tr("libRocket project (*.rproj)"));

    if (!file_path.isEmpty())
    {
        openProject(file_path);
    }
}

void Rockete::menuSaveClicked()
{
    OpenedFile *current_file;
    QString tab_text = ui.codeTabWidget->tabText(ui.codeTabWidget->currentIndex());

    if (tab_text.startsWith('*'))
    {
        tab_text = tab_text.remove(0,1);
    }
    if ((current_file = getOpenedFile(tab_text.toAscii().data())))
    {
        fileWatcher->removePath(current_file->fileInfo.filePath());
        current_file->save();
        ui.codeTabWidget->setTabText(ui.codeTabWidget->currentIndex(), tab_text);
        fileWatcher->addPath(current_file->fileInfo.filePath());
        reloadCurrentDocument();
    }
}

void Rockete::menuSaveAsClicked()
{
    OpenedFile *current_file;
    QString tab_text = ui.codeTabWidget->tabText(ui.codeTabWidget->currentIndex());
    QString file_path = QFileDialog::getSaveFileName( this, tr( "Save as..." ), "", tr("libRocket Markup Language (*.rml);;libRocket CSS (*.rcss)") );

    if (tab_text.startsWith("*"))
    {
        tab_text = tab_text.remove(0,1);
    }
    if ((current_file = getOpenedFile(tab_text.toAscii().data())) && !file_path.isEmpty() )
    {
        current_file->saveAs(file_path);
        openFile(file_path);
    }
}

void Rockete::menuCloseClicked()
{
    codeTabRequestClose(ui.codeTabWidget->currentIndex());
}

void Rockete::codeTextChanged()
{
    QString tab_text = ui.codeTabWidget->tabText(ui.codeTabWidget->currentIndex());
    if (!tab_text.startsWith("*"))
    {
        ui.codeTabWidget->setTabText(ui.codeTabWidget->currentIndex(), "*" + tab_text);
    }
}

void Rockete::codeTabChanged( int index )
{
    OpenedDocument *document;
    QString tab_text = ui.codeTabWidget->tabText(index);
    if(isReloadingFile)
    {
        return;
    }

    if (tab_text.startsWith("*"))
    {
        tab_text = tab_text.remove(0,1);
    }
    if ((document = getDocumentFromFileName(tab_text.toAscii().data())))
    {
        renderingView->changeCurrentDocument(document);
        currentDocument = document;
    }
}

void Rockete::codeTabRequestClose( int index, bool must_save )
{
    OpenedFile *current_file;
    OpenedDocument *document;
    OpenedStyleSheet *style_sheet;
    QString tab_text = ui.codeTabWidget->tabText(index);
    QWidget *removed_widget;

    if (tab_text.startsWith("*"))
    {
        tab_text = tab_text.remove(0,1);
    }
    if ((current_file = getOpenedFile(tab_text.toAscii().data()))) 
    {
        fileWatcher->removePath(current_file->fileInfo.filePath());
        if(must_save)
            current_file->save();
        openedFileList.removeOne( current_file );
    }
    if ((document = getDocumentFromFileName(tab_text.toAscii().data())))
    {
        document->selectedElement = NULL;
        fillAttributeView();
        fillPropertyView();
        ToolManager::getInstance().getCurrentTool()->onUnselect();
        documentList.removeOne(document);
    }
    if ((style_sheet = getStyleSheetFromFileName(tab_text.toAscii().data())))
    {
        styleSheetList.removeOne( style_sheet );
    }

    if(document && must_save)
    {
        renderingView->changeCurrentDocument(NULL);
        currentDocument = NULL;
        RocketHelper::unloadDocument(document->rocketDocument);
        repaintRenderingView();
    }

    removed_widget = ui.codeTabWidget->widget(index);
    ui.codeTabWidget->removeTab( index );
    delete(removed_widget);
}

void Rockete::unselectElement()
{
    if (!currentDocument)
    {
        return;
    }
    currentDocument->selectedElement = NULL;
    fillAttributeView();
    fillPropertyView();
    ToolManager::getInstance().getCurrentTool()->onUnselect();
    repaintRenderingView();
}

void Rockete::menuReloadClicked()
{
    reloadCurrentDocument();
}

void Rockete::menuSetScreenSizeClicked()
{
    QList<LocalScreenSizeItem*> item_list;
    QStringList item_string_list;
    int index_to_select = 0;

    item_list.push_back(new LocalScreenSizeItem(480, 320, "iPhone3"));
    item_list.push_back(new LocalScreenSizeItem(960, 640, "iPhone4"));
    item_list.push_back(new LocalScreenSizeItem(800, 600));
    item_list.push_back(new LocalScreenSizeItem(1024, 768));
    item_list.push_back(new LocalScreenSizeItem(1280, 720));
    item_list.push_back(new LocalScreenSizeItem(1600, 1024));
    item_list.push_back(new LocalScreenSizeItem(1920, 1080, "HD"));

    for (int i=0; i<item_list.size(); ++i)
    {
        LocalScreenSizeItem *item = item_list[i];
        item_string_list << item->displayedString;
        if(item->width == RocketSystem::getInstance().getContext()->GetDimensions().x && item->height == RocketSystem::getInstance().getContext()->GetDimensions().y)
        {
            index_to_select = i;
        }
    }

    bool ok;
    QString item_selected = QInputDialog::getItem(this, tr("Set screen size..."), tr("Size: "), item_string_list, index_to_select, false, &ok);
    if (ok && !item_selected.isEmpty())
    {
        foreach (LocalScreenSizeItem * item, item_list)
        {
            if (item_selected == item->displayedString)
            {
                RocketSystem::getInstance().createContext(item->width, item->height);
                currentDocument->selectedElement = NULL;
                currentDocument->rocketDocument = NULL;
                reloadCurrentDocument();
                break;
            }
        }
    }

    foreach (LocalScreenSizeItem * item, item_list)
    {
       delete item;
    }
}

void Rockete::menuLoadFonts()
{
    QStringList files = QFileDialog::getOpenFileNames(
        this,
        "Open font files...",
        NULL,
        "OTF/TTF files (*.otf *.ttf)"
        );

    foreach (const QString & file, files)
    {
        RocketSystem::getInstance().loadFont(file);
    }
}

void Rockete::menuUndoClicked()
{
    ActionManager::getInstance().applyPrevious();
    //reloadCurrentDocument();
    fillAttributeView();
}

void Rockete::menuRedoClicked()
{
    ActionManager::getInstance().applyNext();
    //reloadCurrentDocument();
    fillAttributeView();
}

void Rockete::menuReloadAssetsClicked()
{
    if (currentDocument)
    {
        renderingView->changeCurrentDocument(NULL);
    }

    for(int i = 0; i < ui.codeTabWidget->count(); i++)
    {
        OpenedDocument *document;
        QString tab_text = ui.codeTabWidget->tabText(i);
        if (tab_text.startsWith("*"))
        {
            tab_text = tab_text.remove(0,1);
        }
        if ((document = getDocumentFromFileName(tab_text.toAscii().data())))
        {
            document->selectedElement = NULL;
            RocketHelper::unloadDocument(document->rocketDocument);
        }
    }

    for(int i = 0; i < ui.codeTabWidget->count(); i++)
    {
        OpenedDocument *document;
        QString tab_text = ui.codeTabWidget->tabText(i);
        if (tab_text.startsWith("*"))
        {
            tab_text = tab_text.remove(0,1);
        }
        if ((document = getDocumentFromFileName(tab_text.toAscii().data())))
        {
            if(document == currentDocument)
            {
                currentDocument->rocketDocument = RocketHelper::loadDocument(currentDocument->fileInfo.filePath().toAscii().data());
                currentDocument->rocketDocument->RemoveReference();
                renderingView->changeCurrentDocument(currentDocument);
            }
            else
            {
                document->rocketDocument = RocketHelper::loadDocument(document->fileInfo.filePath().toAscii().data());
                document->rocketDocument->RemoveReference();
            }
        }
    }
}

void Rockete::propertyViewClicked(const QModelIndex &/*index*/)
{
}

void Rockete::attributeViewClicked(const QModelIndex &index)
{
    if (index.column() == 2 && index.internalPointer())
    {
        QString result;
        if (reinterpret_cast<EditionHelper*>(index.internalPointer())->help(result))
        {
            QModelIndex dataIndex;
            dataIndex = ui.attributeTreeView->model()->index(index.row(), 1, QModelIndex());
            ui.attributeTreeView->model()->setData(dataIndex, QVariant(result));
        }
    }
}

void Rockete::menuRecentFileClicked(QAction *action)
{
    openFile(action->text());
}

void Rockete::menuBackgroundChangeColor()
{
    QColor color = QColorDialog::getColor();

    renderingView->SetClearColor( color.redF(), color.greenF(), color.blueF(), 1.0f );
}

void Rockete::menuBackgroundChangeImage()
{
    QString file_path = QFileDialog::getOpenFileName(this, tr("Open image file..."), "", tr("Image files (*.png *.tga)")); // support jpeg?

    if (!file_path.isEmpty())
    {
        Settings::setBackroundFileName( file_path );
        repaintRenderingView();
    }
}

void Rockete::searchBoxActivated()
{
    OpenedFile *current_file;
    QString tab_text = ui.codeTabWidget->tabText(ui.codeTabWidget->currentIndex());

    if (tab_text.startsWith("*"))
    {
        tab_text = tab_text.remove(0,1);
    }
    if ((current_file = getOpenedFile(tab_text.toAscii().data())))
    {
        if(!searchBox->currentText().isEmpty())
        {
            current_file->find(searchBox->currentText());
        }

        current_file->highlightString(searchBox->currentText());
    }
}

void Rockete::fileTreeDoubleClicked(QTreeWidgetItem *item, int column)
{
    if(item->text(column).endsWith("rml") || item->text(column).endsWith("rcss") || item->text(column).endsWith("txt") || item->text(column).endsWith("rproj"))
    {
        openFile(item->text(column));
    }
}

void Rockete::fileHasChanged(const QString &path)
{
    QFileInfo file_info = path;
    int tab_index = getTabIndexFromFileName(file_info.fileName().toAscii().data());
    int new_tab_index;
    QWidget *widget;
    QMessageBox *message_box = NULL;
   
    message_box = new QMessageBox("Rockete: file change detected", path + " has been modified,\ndo you want to reload it?",QMessageBox::Question, QMessageBox::Yes, QMessageBox::No, NULL, this);
    
    if(message_box->exec()==QMessageBox::No)
    {
        delete(message_box);
        message_box = NULL;
        return;
    }
    delete(message_box);
    message_box = NULL;
    isReloadingFile = true;
    codeTabRequestClose(tab_index, false);
    openFile(path);
    new_tab_index = getTabIndexFromFileName(file_info.fileName().toAscii().data());

    widget = ui.codeTabWidget->widget(new_tab_index);
    ui.codeTabWidget->removeTab(new_tab_index);
    ui.codeTabWidget->insertTab(tab_index,widget,file_info.fileName());
    ui.codeTabWidget->setCurrentIndex(tab_index);
    isReloadingFile = false;
}

// Protected:

void Rockete::keyPressEvent(QKeyEvent *event)
{
    renderingView->keyPressEvent(event);
}

// Private:

void Rockete::openFile(const QString &filePath)
{
    QFileInfo file_info(filePath);
    bool success = false;
    int new_tab_index;

    if(!file_info.exists())
    {
        file_info = Settings::getInterfacePath() + filePath;

        if(!file_info.exists())
        {
            file_info = Settings::getWordListsPath() + filePath;
            
            if(!file_info.exists())
            {
                printf("TODO: Rockete::openFile recursion");
                return;
            }
        }
    }

    for (int i = 0; i < openedFileList.size(); ++i)
    {
        if (openedFileList[i]->fileInfo.filePath() == file_info.filePath())
        {
            ui.codeTabWidget->setCurrentIndex(getTabIndexFromFileName(file_info.fileName().toAscii().data()));
            return;
        }
    }


    if (file_info.suffix() == "rml")
    {
        new_tab_index = openDocument(file_info.filePath().toAscii().data());
        renderingView->changeCurrentDocument(documentList.last());
        currentDocument = documentList.last();
        ui.codeTabWidget->setCurrentIndex(new_tab_index);
        success = true;
    }
    else if (file_info.suffix() == "rcss")
    {
        new_tab_index = openStyleSheet(file_info.filePath().toAscii().data());
        ui.codeTabWidget->setCurrentIndex(new_tab_index);
        success = true;
    }
    else if (file_info.suffix() == "rproj" || file_info.suffix() == "txt")
    {
        new_tab_index = openASCIIFile(file_info.filePath().toAscii().data());
        ui.codeTabWidget->setCurrentIndex(new_tab_index);
        success = true;
    }

    if (success)
    {
        fileWatcher->addPath(file_info.filePath());
        Settings::setMostRecentFile(file_info.filePath());
        generateMenuRecent();
    }
}

void Rockete::openProject(const QString &filePath)
{
    QFileInfo file_info(filePath);

    if (file_info.suffix() == "rproj")
    {
        QFile 
            project_file(filePath);
        QStringList
            path_list;

        ui.treeWidget->clear();

        project_file.open(QFile::ReadOnly);
        while (!project_file.atEnd())
        {
            QByteArray line = project_file.readLine();
            if (!line.isEmpty())
            {
                line = line.trimmed();
                if(!line.endsWith( '/' ))
                {
                    line.append('/');
                }

                if(line.contains('\\'))
                {
                    QMessageBox msgBox;
                    msgBox.setText("project invalid. Only use / in your paths");
                    msgBox.exec();
                    return;
                }

                path_list << line;
            }
        }

        if(path_list.count() < 4)
        {
            QMessageBox msgBox;
            msgBox.setText("project invalid. please provide 4 lines of text with:\nfont path\ntexture path\nrml/rcss path\nkeyword lists path");
            msgBox.exec();
            return;
        }

        Settings::setFontPath(path_list[0]);
        Settings::setTexturePath(path_list[1]);
        Settings::setInterfacePath(path_list[2]);
        Settings::setWordListsPath(path_list[3]);
        Settings::setProject(filePath);

        populateTreeView("Fonts", path_list[0]);
        populateTreeView("Textures", path_list[1]);
        populateTreeView("Interfaces", path_list[2]);
        populateTreeView("Word Lists", path_list[3]);

        RocketSystem::getInstance().loadFonts(Settings::getFontPath());
    }
}

int Rockete::openDocument(const char *file_path)
{
    OpenedDocument *new_document;
    QFileInfo file_info(file_path);

    if (!file_info.exists())
    {
        // :TODO: display error message
        return -1;
    }

    new_document = new OpenedDocument;
    new_document->rocketDocument = RocketHelper::loadDocument(file_path);
    new_document->rocketDocument->RemoveReference();

    if (documentList.isEmpty() && styleSheetList.isEmpty())
    {
        ui.codeTabWidget->clear();
    }

    new_document->fileInfo = file_info;

    documentList.push_back(new_document);
    openedFileList.push_back(new_document);

    new_document->initialize();
    return ui.codeTabWidget->addTab(new_document->textEdit, file_info.fileName());
}

int Rockete::openStyleSheet(const char *file_path)
{
    OpenedStyleSheet *new_style_sheet;
    QFileInfo file_info(file_path);

    new_style_sheet = new OpenedStyleSheet;

    if (documentList.isEmpty() && styleSheetList.isEmpty())
    {
        ui.codeTabWidget->clear();
    }

    new_style_sheet->fileInfo = file_info;

    styleSheetList.push_back(new_style_sheet);
    openedFileList.push_back(new_style_sheet);

    new_style_sheet->initialize();
    return ui.codeTabWidget->addTab(new_style_sheet->textEdit, file_info.fileName());
}

int Rockete::openASCIIFile(const char *file_path)
{
    OpenedFile *new_ascii_file;
    QFileInfo file_info(file_path);

    new_ascii_file = new OpenedFile;
      
    new_ascii_file->fileInfo = file_info;

    openedFileList.push_back(new_ascii_file);

    new_ascii_file->initialize();
    return ui.codeTabWidget->addTab(new_ascii_file->textEdit, file_info.fileName());
}

void Rockete::generateMenuRecent()
{
    ui.menuRecent->clear();
    QStringList recentFileList = Settings::getRecentFileList();

    foreach(const QString &item, recentFileList)
    {
        recentFileActionList.append(ui.menuRecent->addAction(item));
    }
}

void Rockete::populateTreeView(const QString &top_item_name, const QString &directory_path)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui.treeWidget, QStringList(top_item_name));
    QList<QTreeWidgetItem *> items;

    QDirIterator directory_walker(directory_path, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);

    while(directory_walker.hasNext())
    {
        directory_walker.next();

        if(!directory_walker.fileInfo().isDir() && !directory_walker.fileInfo().isHidden())
        {
            items.append(new QTreeWidgetItem(item, QStringList(directory_walker.fileInfo().fileName())));
        }
    }

    item->addChildren(items);
    ui.treeWidget->addTopLevelItem(item);
}

Rockete *Rockete::instance = NULL;
