#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include "Action.h"
#include <QList>
#include <QString>

class ActionManager
{
public:
    ActionManager();
    ~ActionManager();
    static ActionManager & getInstance()
    {
        static ActionManager instance;
        return instance;
    }
    void applyNew(Action * action);
    void applyPrevious();
    void applyNext();

private:
    QList<Action*> previousActionList;
    QList<Action*> nextActionList;
};

#endif
