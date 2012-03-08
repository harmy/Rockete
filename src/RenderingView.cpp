#include "RenderingView.h"

#include <QtGui/QMouseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QUrl>
#include "Rocket/Core/Types.h"
#include "RocketSystem.h"
#include "GraphicSystem.h"
#include "RocketHelper.h"
#include "ToolManager.h"

// Public:

RenderingView::RenderingView(QWidget *parent) : QGLWidget(parent) 
{
    setMouseTracking(true);
    setAcceptDrops(true);
    currentDocument = NULL;
    itMustUpdatePositionOffset = false;
    positionOffset.x=0;
    positionOffset.y=0;
}

void RenderingView::keyPressEvent(QKeyEvent* event)
{
    if (!currentDocument)
        return;

    switch (event->key()) {
    case Qt::Key_Escape:
        currentDocument->selectedElement = 0;
        repaint();
        break;

    case Qt::Key_Delete:
        if (currentDocument->selectedElement) {
            currentDocument->selectedElement->GetParentNode()->RemoveChild(currentDocument->selectedElement);
            Rockete::getInstance().unselectElement();
        }
        break;

    default:
        event->ignore();
        break;
    }
}

void RenderingView::changeCurrentDocument(OpenedDocument *document)
{
    if (currentDocument && currentDocument->rocketDocument)
        currentDocument->rocketDocument->Hide();

    currentDocument = document;

    if (document)
        document->rocketDocument->Show();

    repaint();
}

void RenderingView::reloadDocument()
{
    if(currentDocument->rocketDocument)
        currentDocument->rocketDocument->Hide();
    
    currentDocument->selectedElement = NULL;
    
    if(currentDocument->rocketDocument)
        RocketHelper::unloadDocument(currentDocument->rocketDocument);

    currentDocument->rocketDocument = RocketHelper::loadDocumentFromMemory(currentDocument->toPlainText());
    currentDocument->rocketDocument->RemoveReference();
    currentDocument->rocketDocument->Show();
    repaint();
}

void RenderingView::SetClearColor( float red, float green, float blue, float alpha )
{
    glClearColor( red, green, blue, alpha );
    repaint();
}

// Public slots:

void RenderingView::zoomIn()
{
    GraphicSystem::scaleFactor += 0.1f;
    if(GraphicSystem::scaleFactor > 2.0f)
    {
        GraphicSystem::scaleFactor = 2.0f;
    }
    Rockete::getInstance().setZoomLevel(GraphicSystem::scaleFactor);
    repaint();
}

void RenderingView::zoomOut()
{
    GraphicSystem::scaleFactor -= 0.1f;
    if(GraphicSystem::scaleFactor < 0.0f)
    {
        GraphicSystem::scaleFactor = 0.0f;
    }
    Rockete::getInstance().setZoomLevel(GraphicSystem::scaleFactor);
    repaint();
}

void RenderingView::zoomReset()
{
    GraphicSystem::scaleFactor = 1.0f;
    Rockete::getInstance().setZoomLevel(GraphicSystem::scaleFactor);
    repaint();
}

// Protected:

void RenderingView::initializeGL() 
{
    GraphicSystem::initialize();
}

void RenderingView::resizeGL(int w, int h) 
{
    GraphicSystem::resize(w, h);
}

void RenderingView::paintGL() 
{
    glDisable(GL_SCISSOR_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    RocketSystem::getInstance().getContext()->Update();

    glScalef(GraphicSystem::scaleFactor,GraphicSystem::scaleFactor,1.0f);
    glTranslatef(positionOffset.x,positionOffset.y,0.0f);
    GraphicSystem::scissorOffset = positionOffset;

    glEnable(GL_TEXTURE_2D);
    glDisable( GL_BLEND );
    GraphicSystem::drawBackground();
    glEnable( GL_BLEND );
    glDisable(GL_TEXTURE_2D);
    drawGrid();

    RocketSystem::getInstance().getContext()->Render();

    glDisable(GL_TEXTURE_2D);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(GraphicSystem::scaleFactor,GraphicSystem::scaleFactor,1.0f);
    glTranslatef(positionOffset.x,positionOffset.y,0.0f);

    if (!currentDocument)
        return;

    ToolManager::getInstance().getCurrentTool()->onRender();
}

void RenderingView::mousePressEvent(QMouseEvent *event) 
{
    Rockete::getInstance().setFocus(Qt::MouseFocusReason);

    if (!currentDocument)
        return;

    if (event->button()==Qt::MidButton) {
        startMousePosition.x = event->x();
        startMousePosition.y = event->y();
        oldPositionOffset = positionOffset;
        itMustUpdatePositionOffset=true;
        return;
    }

    Vector2f mouse_position = getMousePositionInDocument(event->x(), event->y());

    RocketSystem::getInstance().getContext()->ProcessMouseButtonDown(event->button()-1, 0);

    ToolManager::getInstance().getCurrentTool()->onMousePress(event->button(), mouse_position);

    repaint();
}

void RenderingView::mouseReleaseEvent(QMouseEvent *event) 
{
    if (event->button()==Qt::MidButton) {
        itMustUpdatePositionOffset=false;
        return;
    }

    Vector2f mouse_position = getMousePositionInDocument(event->x(), event->y());

    RocketSystem::getInstance().getContext()->ProcessMouseButtonUp(event->button()-1, 0);

    ToolManager::getInstance().getCurrentTool()->onMouseRelease(event->button(), mouse_position);

    repaint();
}

void RenderingView::mouseMoveEvent(QMouseEvent *event) 
{
    if (itMustUpdatePositionOffset) {
        mousePositionOffset.x=event->x()-startMousePosition.x;
        mousePositionOffset.y=event->y()-startMousePosition.y;
        positionOffset=oldPositionOffset+mousePositionOffset;
        repaint();
        return;
    }

    Vector2f mouse_position = getMousePositionInDocument(event->x(), event->y());

    RocketSystem::getInstance().getContext()->ProcessMouseMove(mouse_position.x,mouse_position.y, 0);

    ToolManager::getInstance().getCurrentTool()->onMouseMove(mouse_position);

    repaint();
}

void RenderingView::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls() && ToolManager::getInstance().getCurrentTool()->acceptsDrop())
        event->acceptProposedAction();
}

void RenderingView::dropEvent(QDropEvent *event)
{
    QString result = event->mimeData()->urls()[0].toString();
    ToolManager::getInstance().getCurrentTool()->onFileDrop(result);
}

void RenderingView::wheelEvent(QWheelEvent *event)
{
    if ( event->delta() > 0 )
    {
        zoomIn();
    }
    else
    {
        zoomOut();
    }
}

// Private:

void RenderingView::drawGrid()
{
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINES);

    GraphicSystem::putXAxisVertices(0);
    GraphicSystem::putXAxisVertices(RocketSystem::getInstance().getContext()->GetDimensions().y);
    GraphicSystem::putYAxisVertices(0);
    GraphicSystem::putYAxisVertices(RocketSystem::getInstance().getContext()->GetDimensions().x);

    glEnd();
}