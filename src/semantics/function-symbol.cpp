#include "function-symbol.h"

USE_SWIFT_NS


FunctionSymbol::FunctionSymbol(const std::wstring& name)
:name(name)
{

}
const std::wstring& FunctionSymbol::getName() const
{
    return name;
}
const TypePtr& FunctionSymbol::getReturnType()const
{
    return returnType;
}