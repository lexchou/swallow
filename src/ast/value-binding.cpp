
#include "value-binding.h"
USE_SWIFT_NS


ValueBinding::ValueBinding()
    :Statement(0)
{
}
void ValueBinding::serialize(std::wostream& out)
{
    out<<value;
}

