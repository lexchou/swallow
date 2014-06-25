
#include "fallthrough-statement.h"
USE_SWIFT_NS


FallthroughStatement::FallthroughStatement()
    :Statement(0)
{
}
void FallthroughStatement::serialize(std::wostream& out)
{
    out<<L"fallthrough";
}

