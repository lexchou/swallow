#include "function-overloaded-symbol.h"
#include "function-symbol.h"
#include "type.h"
USE_SWIFT_NS

FunctionOverloadedSymbol::FunctionOverloadedSymbol(const std::wstring& name)
:name(name)
{

}
const std::wstring& FunctionOverloadedSymbol::getName() const
{
    return name;
}
FunctionSymbolPtr FunctionOverloadedSymbol::lookupOverload(int argc, TypePtr argv[])
{
    for(const FunctionSymbolPtr& func : functions)
    {
        TypePtr signature = func->getType();
        const std::vector<TypePtr>& parameterTypes = signature->getParameterTypes();

        if(parameterTypes.size() != argc)
            continue;
        //check each parameter's type
        bool matched = true;
        int i = 0;
        for(const TypePtr& parameterType : parameterTypes)
        {
            if(*parameterType != *argv[i++])
            {
                matched = false;
                break;
            }
        }
        if(matched)
            return func;
    }
    return nullptr;
}
void FunctionOverloadedSymbol::add(const FunctionSymbolPtr& func)
{
    functions.push_back(func);
}