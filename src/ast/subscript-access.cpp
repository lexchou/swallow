#include "subscript-access.h"
USE_SWIFT_NS


SubscriptAccess::SubscriptAccess(Expression* self, Expression* index)
    :Expression(2)
{
    setSelf(self);
    setIndex(index);
}
void SubscriptAccess::setSelf(Expression* self)
{
    set(0, self);
}
Expression* SubscriptAccess::getSelf()
{
    return static_cast<Expression*>(get(0));
}

void SubscriptAccess::setIndex(Expression* index)
{
    set(1, index);
}
Expression* SubscriptAccess::getIndex()
{
    return static_cast<Expression*>(get(1));
}
void SubscriptAccess::setIndex(int idx, Expression* index)
{
    set(idx + 1, index);
}
Expression* SubscriptAccess::getIndex(int idx)
{
    return static_cast<Expression*>(get(idx + 1));
}

void SubscriptAccess::addIndex(Expression* index)
{
    children.push_back(index);
}
int SubscriptAccess::numIndices()
{
    return children.size() - 1;
}


void SubscriptAccess::serialize(std::wostream& out)
{
    getSelf()->serialize(out);
    out<<L"[";
    for(int i = 0; i < numIndices(); i++)
    {
        Expression* index = getIndex(i);
        if(i != 0)
            out<<L", ";
        index->serialize(out);
    }
    out<<L"]";
}

