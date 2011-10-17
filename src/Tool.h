#ifndef TOOL_H
#define TOOL_H

#include <QObject>
#include <QString>
#include <QLayout>
#include <QAction>
#include <QWidget>
#include "RocketHelper.h"

class Tool : public QObject
{
    Q_OBJECT

public:
    struct Marker
    {
        Marker(const Vector2f &_position, const Vector2f &_extent) : position(_position), extent(_extent) {}
        Marker(const Vector2f &_position, const float _size) : position(_position), extent(_size,_size) {}
        Marker(const float _x, const float _y, const float _size) : position(_x, _y), extent(_size,_size) {}
        Vector2f position;
        Vector2f extent;
    };
    Tool();
    virtual ~Tool();
    const QString & getName() const { return name; }
    const QString & getImageName() const { return imageName; }
    QAction *getAction() { return action; }
    const QList<Marker> & getMarkerList() const; 
    void onSelect();
    void setAction(QAction *_action) { action = _action; }
    virtual void onElementClicked(Element *){}
    virtual void onRender(){}
    virtual void onMousePress(const Qt::MouseButton, const Vector2f &) {}
    virtual void onMouseRelease(const Qt::MouseButton, const Vector2f &) {}
    virtual void onMouseMove(const Vector2f &) {}
    virtual void onUnselect() {}

public slots:
    void onActionTriggered();

protected:
    void renderMarkers() const;
    bool findMarkerFromPosition(int& markerIndex, const Vector2f &position) const;

    QString name;
    QString imageName;
    QWidget *widget;
    QAction *action;
    QList<Marker> markerList; 
};


#endif