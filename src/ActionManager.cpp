#include "ActionManager.h"

ActionManager::ActionManager()
{

}

ActionManager::~ActionManager()
{
    for(int i=0;i<previousActionList.size();++i) {
        delete previousActionList[i];
    }

    for(int i=0;i<nextActionList.size();++i) {
        delete nextActionList[i];
    }
}

void ActionManager::applyNew(Action * action)
{
    action->apply();
    previousActionList.push_back(action);
}

void ActionManager::applyPrevious()
{
    if (!previousActionList.isEmpty()) {
        Action *action = previousActionList.takeLast();
        action->unapply();
        nextActionList.push_back(action);
    }
}

void ActionManager::applyNext()
{
    if (!nextActionList.isEmpty()) {
        Action *action = nextActionList.takeLast();
        action->apply();
        previousActionList.push_back(action);
    }
}
