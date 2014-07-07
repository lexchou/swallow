#include "subscript-access.h"
USE_SWIFT_NS


SubscriptAccess::SubscriptAccess(Expression* self, Expression* index)
{
    setSelf(self);
    setIndex(index);
}

SubscriptAccess::~SubscriptAccess()
{
    SafeDelete(self);
    SafeDeleteAll(indices);
}

void SubscriptAccess::setSelf(Expression* self)
{
    this->self = self;
}
Expression* SubscriptAccess::getSelf()
{
    return self;
}

void SubscriptAccess::setIndex(Expression* index)
{
    SafeDeleteAll(indices);
    addIndex(index);
}
Expression* SubscriptAccess::getIndex()
{
    if(indices.empty())
        return NULL;
    return indices.front();
}
void SubscriptAccess::setIndex(int idx, Expression* index)
{
    indices[idx] = index;
}
Expression* SubscriptAccess::getIndex(int idx)
{
    return indices[idx];
}

void SubscriptAccess::addIndex(Expression* index)
{
    indices.push_back(index);
}
int SubscriptAccess::numIndices()
{
    return indices.size();
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

