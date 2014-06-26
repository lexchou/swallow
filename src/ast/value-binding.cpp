
#include "value-binding.h"
USE_SWIFT_NS


ValueBinding::ValueBinding()
    :Statement(1)
{
}

void ValueBinding::setBinding(Statement* st)
{
    set(0, st);
}
Statement* ValueBinding::getBinding()
{
    return static_cast<Statement*>(get(0));
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
