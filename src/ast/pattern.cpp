
#include "pattern.h"
USE_SWIFT_NS


Pattern::Pattern()
    :Statement(0)
{
}
void Pattern::serialize(std::wostream& out)
{
    out<<value;
}

