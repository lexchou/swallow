
#include "fallthrough-statement.h"
USE_SWIFT_NS


FallthroughStatement::FallthroughStatement()
    :Statement(NodeType::Fallthrough)
{
}
void FallthroughStatement::serialize(std::wostream& out)
{
    out<<L"fallthrough";
}

