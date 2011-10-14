#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include <QString>
#include "Action.h"

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
