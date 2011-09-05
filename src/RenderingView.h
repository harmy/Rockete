#ifndef RENDERINGVIEW_H
#define RENDERINGVIEW_H

#include <QtOpenGL/QGLWidget>
#include <Rocket/Core.h>
#include "OpenedDocument.h"
#include "GraphicSystem.h"

class RenderingView : public QGLWidget 
{
    Q_OBJECT

public:
    RenderingView(QWidget *parent = NULL);
    void keyPressEvent(QKeyEvent *event);
    void changeCurrentDocument(OpenedDocument * document);

public slots:
    void zoomIn();
    void zoomOut();
    void zoomReset();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    void drawGrid();
    void drawBackground();
    inline Vector2f getMousePositionInDocument(const int x, const int y) const {
        Vector2f result;

        result.x = x/GraphicSystem::scaleFactor-int(positionOffset.x);
        result.y = y/GraphicSystem::scaleFactor-int(positionOffset.y);

        return result;
    }

    OpenedDocument * currentDocument;
    Vector2f positionOffset;
    Vector2f oldPositionOffset;
    Vector2f startMousePosition;
    Vector2f mousePositionOffset;
    bool itMustUpdatePositionOffset;
};

#endif