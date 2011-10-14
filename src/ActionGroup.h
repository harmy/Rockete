#ifndef ACTIONGROUP_H
#define ACTIONGROUP_H

#include "Action.h"
#include <QList>

class ActionGroup : public Action
{
public:
    ActionGroup();
    ActionGroup(QList<Action*> &actionList);
    virtual void apply();
    virtual void unapply();
    void add(Action *action);

private:

    QList<Action*> actionList;
};

#endif
