#include "ActionGroup.h"

ActionGroup::ActionGroup()
{
    type = ActionTypeGroup;
}

ActionGroup::ActionGroup(QList<Action*> &action_list)
: actionList(action_list)
{
    type = ActionTypeGroup;
}

void ActionGroup::apply()
{
    for (int i=0; i<actionList.size(); ++i)
    {
        actionList[i]->apply();
    }
}

void ActionGroup::unapply()
{
    for (int i=actionList.size()-1; i>=0; --i)
    {
        actionList[i]->unapply();
    }
}

void ActionGroup::add(Action *action)
{
    actionList.push_back(action);
}
