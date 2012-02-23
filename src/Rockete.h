#ifndef ROCKETE_H
#define ROCKETE_H

#include <QtGui/QMainWindow>
#include <QFileSystemWatcher>
#include <QLabel>
#include "ui_rockete.h"
#include "RenderingView.h"
#include "OpenedDocument.h"
#include "OpenedStyleSheet.h"
#include "AttributeTreeModel.h"
#include "PropertyTreeModel.h"

class Rockete : public QMainWindow
{
    Q_OBJECT

public:
    Rockete(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~Rockete();
    static Rockete & getInstance() {
        return *instance;
    }
    void repaintRenderingView();
    void fillAttributeView();
    void fillPropertyView();
    void selectElement(Element *element);
    void clear();
    void reloadCurrentDocument();
    int getTabIndexFromFileName(const char * name);
    //OpenedDocument *getDocumentFromTabIndex(const int tab_index);
    //OpenedStyleSheet *getStyleSheetFromTabIndex(const int tab_index);
    OpenedDocument *getDocumentFromFileName(const char *name);
    OpenedStyleSheet *getStyleSheetFromFileName(const char *name);
    OpenedFile *getOpenedFile(const char * file_path, const bool try_to_open=false);
    //OpenedFile *getOpenedFileFromTabIndex(const int tab_index);
    QWidget *getCurrentToolTab() { return ui.currentToolTab; }
    OpenedDocument *getCurrentDocument() { return currentDocument; }
    QString getPathForFileName(const QString &filename);
    void setZoomLevel(float level);

public slots:
    void menuOpenClicked();
    void menuOpenProjectClicked();
    void menuSaveClicked();
    void menuSaveAsClicked();
    void menuCloseClicked();
    void codeTextChanged();
    void codeTabChanged(int index);
    void codeTabRequestClose(int index, bool must_save = true);
    void unselectElement();
    void menuReloadClicked();
    void menuSetScreenSizeClicked();
    void menuLoadFonts();
    void menuUndoClicked();
    void menuRedoClicked();
    void menuReloadAssetsClicked();
    void propertyViewClicked(const QModelIndex &index);
    void attributeViewClicked(const QModelIndex &index);
    void menuRecentFileClicked(QAction *action);
    void menuBackgroundChangeColor();
    void menuBackgroundChangeImage();
    void searchBoxActivated();
    void languageBoxActivated();
    void fileTreeDoubleClicked(QTreeWidgetItem *item, int column);
    void fileHasChanged(const QString &path);

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void changeEvent(QEvent *event); 
    virtual void closeEvent(QCloseEvent *event);

private:
    void openFile(const QString &file_path);
    void openProject(const QString &filePath);
    int openDocument(const char *);
    int openStyleSheet(const char *);
    int openASCIIFile(const char *);
    void generateMenuRecent();
    void populateTreeView(const QString &top_item_name, const QString &directory_path);
    void loadPlugins();

    Ui::rocketeClass ui;
    RenderingView *renderingView;
    QList<OpenedDocument*> documentList;
    QList<OpenedStyleSheet*> styleSheetList;
    QList<OpenedFile*> openedFileList;
    OpenedDocument *currentDocument;
    AttributeTreeModel *attributeTreeModel;
    PropertyTreeModel *propertyTreeModel;
    QList<QAction*> recentFileActionList;
    static Rockete *instance;
    QComboBox *searchBox;
    QComboBox *languageBox;
    QLabel *labelZoom;
    QFileSystemWatcher *fileWatcher;
    bool isReloadingFile;
    QMap<int, QString> fileChangedOutsideArray;
};

#endif // ROCKETE_H
