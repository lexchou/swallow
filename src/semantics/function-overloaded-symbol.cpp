#include "function-overloaded-symbol.h"
#include "function-symbol.h"
#include "type.h"
USE_SWIFT_NS

FunctionOverloadedSymbol::FunctionOverloadedSymbol(const std::wstring& name)
:name(name)
{

}
FunctionOverloadedSymbol::FunctionOverloadedSymbol()
{

}
const std::wstring& FunctionOverloadedSymbol::getName() const
{
    return name;
}
int FunctionOverloadedSymbol::numOverloads()const
{
    return functions.size();
}
FunctionSymbolPtr FunctionOverloadedSymbol::lookupOverload(int argc, TypePtr argv[])
{
    for(const FunctionSymbolPtr& func : functions)
    {
        TypePtr signature = func->getType();
        const std::vector<Type::Parameter>& parameterTypes = signature->getParameters();

        if(parameterTypes.size() != argc)
            continue;
        //check each parameter's type
        bool matched = true;
        int i = 0;
        for(const Type::Parameter& parameter : parameterTypes)
        {
            if(*parameter.type != *argv[i++])
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