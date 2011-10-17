#ifndef ACTION_H
#define ACTION_H

class Action
{
public:
    enum Type
    {
        ActionTypeUnknown,
        ActionTypeSetProperty,
        ActionTypeSetInlineProperty,
        ActionTypeSetAttribute,
        ActionTypeInsertElement,
        ActionTypeGroup
    };
    Action();
    virtual ~Action();
    Type getType() const { return type; };
    virtual void apply() = 0;
    virtual void unapply() = 0;

protected:
    Type type;
};

#endif
