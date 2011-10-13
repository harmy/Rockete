#include "RenderingView.h"

#include <QtGui/QMouseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include "Rocket/Core/Types.h"
#include "RocketSystem.h"
#include "GraphicSystem.h"
#include "RocketHelper.h"
#include "ToolManager.h"

// Public:

RenderingView::RenderingView(QWidget *parent) : QGLWidget(parent) 
{
    setMouseTracking(true);
    currentDocument = NULL;
    itMustUpdatePositionOffset = false;
    positionOffset.x=0;
    positionOffset.y=0;
}

void RenderingView::changeCurrentDocument( OpenedDocument * document )
{
    if( currentDocument )
    {
        currentDocument->rocketDocument->Hide();
    }

    currentDocument = document;

    if( document )
    {
        document->rocketDocument->Show();
    }

    repaint();
}

// Public slots:

void RenderingView::zoomIn()
{
    GraphicSystem::scaleFactor += 0.1f;
    repaint();
}

void RenderingView::zoomOut()
{
    GraphicSystem::scaleFactor -= 0.1f;
    repaint();
}

void RenderingView::zoomReset()
{
    GraphicSystem::scaleFactor = 1.0f;
    repaint();
}

// Protected:

void RenderingView::initializeGL() 
{
    GraphicSystem::initialize();
}

void RenderingView::resizeGL(int w, int h) 
{
    GraphicSystem::resize( w, h );
}

void RenderingView::paintGL() 
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    RocketSystem::getInstance().getContext()->Update();

    glEnable( GL_BLEND );

    glScalef(GraphicSystem::scaleFactor,GraphicSystem::scaleFactor,1.0f);
    glTranslatef(positionOffset.x,positionOffset.y,0.0f);
    GraphicSystem::scissorOffset = positionOffset;

    drawGrid();

    RocketSystem::getInstance().getContext()->Render();

    glDisable(GL_TEXTURE_2D);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(GraphicSystem::scaleFactor,GraphicSystem::scaleFactor,1.0f);
    glTranslatef(positionOffset.x,positionOffset.y,0.0f);

    if(!currentDocument)
    {
        return;
    }

    ToolManager::getInstance().getCurrentTool()->onRender();
}

void RenderingView::mousePressEvent(QMouseEvent *event) 
{
    Rockete::getInstance().setFocus(Qt::MouseFocusReason);

    if( !currentDocument )
        return;

    if(event->button()==Qt::MidButton)
    {
        startMousePosition.x = event->x();
        startMousePosition.y = event->y();
        oldPositionOffset = positionOffset;
        itMustUpdatePositionOffset=true;
        return;
    }

    RocketSystem::getInstance().getContext()->ProcessMouseButtonDown(event->button()-1, 0);
    repaint();

    if(event->button() == Qt::RightButton)
    {
        if(currentDocument->selectedElement)
        {
            if(currentDocument->selectedElement && currentDocument->selectedElement->HasAttribute("width") && currentDocument->selectedElement->HasAttribute("height"))
            {
                Rocket::Core::Vector2f
                    new_extent;

                new_extent.x = event->x() - currentDocument->selectedElement->GetAbsoluteOffset().x;
                new_extent.y = event->y() - currentDocument->selectedElement->GetAbsoluteOffset().y;

                currentDocument->selectedElement->SetAttribute< float >("width", new_extent.x);
                currentDocument->selectedElement->SetAttribute< float >("height", new_extent.y);

                repaint();
            }
            else if( currentDocument->selectedElement->GetTagName() == "div" )
            {
                bool
                    ok;
                Rocket::Core::String
                    content;

                currentDocument->selectedElement->GetInnerRML( content );

                QString text = QInputDialog::getText(
                    this,
                    "Edit div",
                    "Content:",
                    QLineEdit::Normal,
                    content.CString(), 
                    &ok
                    );

                if ( ok )
                {
                    currentDocument->selectedElement->SetInnerRML( text.toStdString().c_str() );
                }
            }
        }
    }
}

void RenderingView::mouseReleaseEvent(QMouseEvent *event) 
{
    if(event->button()==Qt::MidButton)
    {
        itMustUpdatePositionOffset=false;
        return;
    }

    RocketSystem::getInstance().getContext()->ProcessMouseButtonUp(event->button()-1, 0);
    repaint();
}

void RenderingView::mouseMoveEvent(QMouseEvent *event) 
{
    if(itMustUpdatePositionOffset)
    {
        mousePositionOffset.x=event->x()-startMousePosition.x;
        mousePositionOffset.y=event->y()-startMousePosition.y;
        positionOffset=oldPositionOffset+mousePositionOffset;
        repaint();
        return;
    }

    Vector2f mouse_position = getMousePositionInDocument(event->x(), event->y());
    RocketSystem::getInstance().getContext()->ProcessMouseMove(mouse_position.x,mouse_position.y, 0);
    repaint();
}

void RenderingView::keyPressEvent(QKeyEvent* event) 
{
    if( !currentDocument )
        return;

    switch(event->key()) 
    {

    case Qt::Key_Escape:
        currentDocument->selectedElement = 0;
        repaint();
        break;

    case Qt::Key_Delete:
        currentDocument->selectedElement->GetParentNode()->RemoveChild( currentDocument->selectedElement );
        currentDocument->selectedElement = 0;
        repaint();
        break;

    default:
        event->ignore();
        break;
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
