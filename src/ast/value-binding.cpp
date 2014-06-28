
#include "value-binding.h"
USE_SWIFT_NS


ValueBinding::ValueBinding()
    :Pattern(1)
{
}

void ValueBinding::setBinding(Pattern* st)
{
    set(0, st);
}
Pattern* ValueBinding::getBinding()
{
    return static_cast<Pattern*>(get(0));
}

void LetBinding::serialize(std::wostream& out)
{
    out<<L"let ";
    getBinding()->serialize(out);
}


void VarBinding::serialize(std::wostream& out)
{
    out<<L"var ";
    getBinding()->serialize(out);
}
