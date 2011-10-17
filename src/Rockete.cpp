#include "Rockete.h"

#include <QFileDialog>
#include <QTextEdit>
#include <QFileInfo>
#include <QInputDialog>
#include <QTextDocument>
#include <QGridLayout>
#include "AttributeTreeModel.h"
#include "RocketSystem.h"
#include "ActionManager.h"
#include "ToolManager.h"
#include "EditionHelper.h"

struct LocalScreenSizeItem
{
    LocalScreenSizeItem(const int _width, const int _height, const char *text=NULL)
    {
        width = _width;
        height = _height;
        displayedString = QString::number(width) + "x" + QString::number(height);

        if (text) {
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
: QMainWindow(parent, flags), currentDocument(NULL)
{
    instance = this;

    ui.setupUi(this);

    ui.currentToolTab->setLayout(new QGridLayout());

    // :TODO: Load recent files

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

    ToolManager::getInstance().setup(ui.mainToolBar, ui.menuTools);

    attributeTreeModel = new AttributeTreeModel();
    propertyTreeModel = new PropertyTreeModel();
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
    ui.attributeTreeView->header()->setResizeMode(1, QHeaderView::ResizeToContents);
}

void Rockete::fillPropertyView()
{
    propertyTreeModel->setupData(currentDocument, currentDocument->selectedElement);
    ui.propertyTreeView->reset();
    ui.propertyTreeView->setModel(propertyTreeModel);
    ui.propertyTreeView->expandAll();
    ui.propertyTreeView->header()->setResizeMode(0, QHeaderView::ResizeToContents);
    ui.propertyTreeView->header()->setResizeMode(1, QHeaderView::ResizeToContents);
}

void Rockete::selectElement(Element *element)
{
    if(element != currentDocument->selectedElement) {
        currentDocument->selectedElement = element;
        renderingView->repaint();
        fillAttributeView();
        fillPropertyView();
    }
}

void Rockete::clear()
{
    for (int i=0;i<openedFileList.size();++i) {
        delete openedFileList[i];
    }

    openedFileList.clear();
    documentList.clear();
    styleSheetList.clear();

    // :TODO: Unload current doc, etc...
}

void Rockete::reloadCurrentDocument()
{
    if (currentDocument) {
        // :TODO: Clean this part.
        renderingView->changeCurrentDocument(NULL);
        currentDocument->selectedElement = NULL;
        currentDocument->rocketDocument->GetStyleSheet()->RemoveReference();
        RocketHelper::unloadDocument(currentDocument->rocketDocument);
        currentDocument->rocketDocument = RocketHelper::loadDocument(currentDocument->fileInfo.filePath().toStdString().c_str());
        renderingView->changeCurrentDocument(currentDocument);
    }
}

OpenedDocument *Rockete::getDocumentFromTabIndex(const int tab_index)
{
    for (int i = 0; i < documentList.size(); ++i) {
        if (documentList[i]->tabIndex == tab_index)
            return documentList[i];
    }

    return NULL;
}

OpenedStyleSheet *Rockete::getStyleSheetFromTabIndex(const int tab_index)
{
    for (int i = 0; i < styleSheetList.size(); ++i) {
        if (styleSheetList[i]->tabIndex == tab_index)
            return styleSheetList[i];
    }

    return NULL;
}

OpenedDocument *Rockete::getDocumentFromFileName(const char * name)
{
    for (int i = 0; i < documentList.size(); ++i) {
        if (documentList[i]->fileInfo.fileName() == name)
            return documentList[i];
    }

    return NULL;
}

OpenedStyleSheet *Rockete::getStyleSheetFromFileName(const char * name)
{
    for (int i = 0; i < styleSheetList.size(); ++i) {
        if (styleSheetList[i]->fileInfo.fileName() == name)
            return styleSheetList[i];
    }

    return NULL;
}

OpenedFile *Rockete::getOpenedFile(const char * file_path, const bool try_to_open)
{
    QFileInfo file_info(file_path);

    for (int i = 0; i < openedFileList.size(); ++i) {
        if (openedFileList[i]->fileInfo.fileName() == file_info.fileName())
            return openedFileList[i];
    }

    if (try_to_open) {
        openFile(file_info.filePath());
        return getOpenedFile(file_path);
    }

    return NULL;
}

OpenedFile *Rockete::getOpenedFileFromTabIndex(const int tab_index)
{
    for (int i = 0; i < openedFileList.size(); ++i) {
        if (openedFileList[i]->tabIndex == tab_index)
            return openedFileList[i];
    }

    return NULL;
}

// Public slots:

void Rockete::menuOpenClicked()
{
    QString file_path = QFileDialog::getOpenFileName(this, tr("Open libRocket file..."), "", tr("libRocket files (*.rml *.rcss)"));

    if (!file_path.isEmpty())
        openFile(file_path);
}

void Rockete::menuSaveClicked()
{
    OpenedFile *current_file;
    if ((current_file = getOpenedFileFromTabIndex(ui.codeTabWidget->currentIndex())))
        current_file->save();
}

void Rockete::menuCloseClicked()
{
    // :TODO:
}

void Rockete::codeTabChanged( int index )
{
    OpenedDocument *document;
    if ((document = getDocumentFromTabIndex(index))) {
        renderingView->changeCurrentDocument(document);
        currentDocument = document;
    }
}

void Rockete::unselectElement()
{
    if (!currentDocument)
        return;

    currentDocument->selectedElement = NULL;
    renderingView->repaint();
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
    int index_to_select;

    // :TODO: Fill the screen size list.
    item_list.push_back(new LocalScreenSizeItem(480, 320, "iPhone3"));
    item_list.push_back(new LocalScreenSizeItem(960, 640, "iPhone4"));
    item_list.push_back(new LocalScreenSizeItem(800, 600));
    item_list.push_back(new LocalScreenSizeItem(1024, 768));
    item_list.push_back(new LocalScreenSizeItem(1280, 720));
    item_list.push_back(new LocalScreenSizeItem(1600, 1024));
    item_list.push_back(new LocalScreenSizeItem(1920, 1080, "HD"));

    for (int i=0; i<item_list.size(); ++i) {
        LocalScreenSizeItem *item = item_list[i];
        item_string_list << item->displayedString;
        if(item->width == RocketSystem::getInstance().getContext()->GetDimensions().x && item->height == RocketSystem::getInstance().getContext()->GetDimensions().y)
            index_to_select = i;
    }

    bool ok;
    QString item_selected = QInputDialog::getItem(this, tr("Set screen size..."), tr("Size: "), item_string_list, index_to_select, false, &ok);
    if (ok && !item_selected.isEmpty()) {
        foreach (LocalScreenSizeItem * item, item_list) {
            if (item_selected == item->displayedString) {
                RocketSystem::getInstance().createContext(item->width, item->height);
                reloadCurrentDocument();
                break;
            }
        }
    }

    foreach (LocalScreenSizeItem * item, item_list) {
       delete item;
    }
}

void Rockete::menuLoadFonts()
{
    QStringList files = QFileDialog::getOpenFileNames(
        this,
        "Open font files...",
        NULL,
        "OTF files (*.otf)"
        );

    foreach (const QString & file, files) {
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

void Rockete::propertyViewClicked(const QModelIndex &/*index*/)
{
}

void Rockete::attributeViewClicked(const QModelIndex &index)
{
    if (index.column() == 2 && index.internalPointer()) {
        QString result;
        reinterpret_cast<EditionHelper*>(index.internalPointer())->help(result);
        ui.attributeTreeView->model()->setData(index.parent().child(index.row(),1), QVariant(result));
    }
}

// Protected:

void Rockete::keyPressEvent(QKeyEvent *event)
{
    renderingView->keyPressEvent(event);
}

// Private:

void Rockete::openFile(const QString &file_path)
{
    QFileInfo file_info(file_path);

    if (file_info.suffix() == "rml") {
        openDocument(file_path.toStdString().c_str());
        renderingView->changeCurrentDocument(documentList.last());
        currentDocument = documentList.last();
        ui.codeTabWidget->setCurrentIndex(currentDocument->tabIndex);
    }
    else if (file_info.suffix() == "rcss") {
        openStyleSheet(file_path.toStdString().c_str());
        ui.codeTabWidget->setCurrentIndex(styleSheetList.last()->tabIndex);
    }
}

void Rockete::openDocument(const char *file_path)
{
    OpenedDocument *new_document;
    QFileInfo file_info(file_path);

    new_document = new OpenedDocument;

    // :TODO: Check if document exists.
    new_document->rocketDocument = RocketHelper::loadDocument(file_path);

    if (documentList.isEmpty() && styleSheetList.isEmpty())
        ui.codeTabWidget->clear();

    new_document->fileInfo = file_info;

    documentList.push_back(new_document);
    openedFileList.push_back(new_document);

    new_document->initialize();
    new_document->tabIndex = ui.codeTabWidget->addTab(new_document->textEdit, file_info.fileName());

}

void Rockete::openStyleSheet(const char *file_path)
{
    OpenedStyleSheet *new_style_sheet;
    QFileInfo file_info(file_path);

    new_style_sheet = new OpenedStyleSheet;

    if (documentList.isEmpty() && styleSheetList.isEmpty())
        ui.codeTabWidget->clear();

    new_style_sheet->fileInfo = file_info;

    styleSheetList.push_back(new_style_sheet);
    openedFileList.push_back(new_style_sheet);

    new_style_sheet->initialize();
    new_style_sheet->tabIndex = ui.codeTabWidget->addTab(new_style_sheet->textEdit, file_info.fileName());

}

Rockete *Rockete::instance = NULL;
