
#include "value-binding.h"
USE_SWIFT_NS


ValueBinding::ValueBinding()
    :binding(NULL)
{
}
ValueBinding::~ValueBinding()
{
    SafeDelete(binding);
}

void ValueBinding::setBinding(Pattern* st)
{
    this->binding = st;
}
Pattern* ValueBinding::getBinding()
{
    return binding;
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
