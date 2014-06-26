
#include "tuple.h"
USE_SWIFT_NS


Tuple::Tuple()
    :Expression(0)
{
}
void Tuple::serialize(std::wostream& out)
{
    out<<value;
}

