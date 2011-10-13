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
    Tool();
    virtual ~Tool();
    const QString & getName() const { return name; }
    const QString & getImageName() const { return imageName; }
    QAction * getAction() { return action; }
    void onSelect();
    void setAction(QAction *_action) { action = _action; }
    virtual void onElementClicked(Element *){}
    virtual void onRender(){}

public slots:
    void onActionTriggered();

protected:
    QString name;
    QString imageName;
    QWidget *widget;
    QAction *action;
};


#endif