#ifndef ROCKETE_H
#define ROCKETE_H

#include <QtGui/QMainWindow>
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
    void selectElement(Element * element);
    void clear();
    void reloadCurrentDocument();
    OpenedDocument *getDocumentFromTabIndex(const int tab_index);
    OpenedStyleSheet *getStyleSheetFromTabIndex(const int tab_index);
    OpenedDocument *getDocumentFromFileName(const char * name);
    OpenedStyleSheet *getStyleSheetFromFileName(const char * name);
    OpenedFile *getOpenedFile(const char * file_path, const bool try_to_open=false);
    OpenedFile *getOpenedFileFromTabIndex(const int tab_index);
    QWidget *getCurrentToolTab() { return ui.currentToolTab; }
    OpenedDocument *getCurrentDocument() { return currentDocument; }

protected:
    virtual void keyPressEvent(QKeyEvent * e);

private slots:
    void menuOpenClicked();
    void menuSaveClicked();
    void menuCloseClicked();
    void codeTabChanged(int index);
    void unselectElement();
    void menuReloadClicked();
    void menuSetScreenSizeClicked();
    void menuLoadFonts();
    void menuUndoClicked();
    void menuRedoClicked();
    void propertyViewClicked(const QModelIndex & index);

private:
    void openFile(const QString & file_path);
    void openDocument(const char *);
    void openStyleSheet(const char *);

    Ui::rocketeClass ui;
    RenderingView * renderingView;
    QList<OpenedDocument*> documentList;
    QList<OpenedStyleSheet*> styleSheetList;
    QList<OpenedFile*> openedFileList;
    OpenedDocument * currentDocument;
    AttributeTreeModel *attributeTreeModel;
    PropertyTreeModel *propertyTreeModel;
    static Rockete *instance;
};

#endif // ROCKETE_H
