
#include "fallthrough-statement.h"
#include "node-visitor.h"
USE_SWIFT_NS


FallthroughStatement::FallthroughStatement()
    :Statement(NodeType::Fallthrough)
{
}
void FallthroughStatement::serialize(std::wostream& out)
{
    out<<L"fallthrough";
}
void FallthroughStatement::accept(NodeVisitor* visitor)
{
    visitor->visitFallthrough(this);
}
