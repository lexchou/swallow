
#include "fallthrough-statement.h"
USE_SWIFT_NS


FallthroughStatement::FallthroughStatement()
{
}
void FallthroughStatement::serialize(std::wostream& out)
{
    out<<L"fallthrough";
}

