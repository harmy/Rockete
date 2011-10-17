#ifndef RENDERINGVIEW_H
#define RENDERINGVIEW_H

#include <QtOpenGL/QGLWidget>
#include <Rocket/Core.h>
#include "OpenedDocument.h"
#include "GraphicSystem.h"
#include <QDropEvent>
#include <QDragEnterEvent>

class RenderingView : public QGLWidget 
{
    Q_OBJECT

public:
    RenderingView(QWidget *parent = NULL);
    void keyPressEvent(QKeyEvent *event);
    void changeCurrentDocument(OpenedDocument *document);

public slots:
    void zoomIn();
    void zoomOut();
    void zoomReset();

protected:
    void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent *event);


private:
    void drawGrid();
    void drawBackground();
    inline Vector2f getMousePositionInDocument(const int x, const int y) const {
        Vector2f result;

        result.x = x / GraphicSystem::scaleFactor - int(positionOffset.x);
        result.y = y / GraphicSystem::scaleFactor - int(positionOffset.y);

        return result;
    }

    OpenedDocument *currentDocument;
    Vector2f positionOffset;
    Vector2f oldPositionOffset;
    Vector2f startMousePosition;
    Vector2f mousePositionOffset;
    bool itMustUpdatePositionOffset;
};

#endif