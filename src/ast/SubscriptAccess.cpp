#include "SubscriptAccess.h"
#include "NodeVisitor.h"
USE_SWIFT_NS


SubscriptAccess::SubscriptAccess()
    :Expression(NodeType::SubscriptAccess)
{

}

SubscriptAccess::~SubscriptAccess()
{
}

void SubscriptAccess::setSelf(ExpressionPtr self)
{
    this->self = self;
}
ExpressionPtr SubscriptAccess::getSelf()
{
    return self;
}

void SubscriptAccess::setIndex(ExpressionPtr index)
{
    indices.clear();
    addIndex(index);
}
ExpressionPtr SubscriptAccess::getIndex()
{
    if(indices.empty())
        return NULL;
    return indices.front();
}
void SubscriptAccess::setIndex(int idx, const ExpressionPtr& index)
{
    indices[idx] = index;
}
ExpressionPtr SubscriptAccess::getIndex(int idx)
{
    return indices[idx];
}

void SubscriptAccess::addIndex(ExpressionPtr index)
{
    indices.push_back(index);
}
int SubscriptAccess::numIndices()
{
    return indices.size();
}

void SubscriptAccess::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitSubscriptAccess);
}


