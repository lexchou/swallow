#include "FunctionSymbol.h"
#include "Type.h"
#include <cassert>

USE_SWIFT_NS


FunctionSymbol::FunctionSymbol(const std::wstring& name, const TypePtr& functionType, const FunctionDefPtr& definition)
:name(name), type(functionType), definition(definition)
{
    assert(functionType);
    assert(functionType->getCategory() == Type::Function);
}

TypePtr FunctionSymbol::getType()
{
    return type;
}
const std::wstring& FunctionSymbol::getName() const
{
    return name;
}
TypePtr FunctionSymbol::getReturnType()const
{
    return type->getReturnType();
}
FunctionDefPtr FunctionSymbol::getDefinition()
{
    return definition.lock();
}
