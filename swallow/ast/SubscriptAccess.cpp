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

void SubscriptAccess::setSelf(const ExpressionPtr& self)
{
    this->self = self;
}
const ExpressionPtr& SubscriptAccess::getSelf()
{
    return self;
}

void SubscriptAccess::setIndex(const ParenthesizedExpressionPtr& index)
{
    this->indices = index;
}
const ParenthesizedExpressionPtr& SubscriptAccess::getIndex()
{
    return indices;
}


void SubscriptAccess::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitSubscriptAccess);
}


