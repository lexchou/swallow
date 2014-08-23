#include "FallthroughStatement.h"
#include "NodeVisitor.h"
USE_SWIFT_NS


FallthroughStatement::FallthroughStatement()
    :Statement(NodeType::Fallthrough)
{
}

void FallthroughStatement::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitFallthrough);
}
