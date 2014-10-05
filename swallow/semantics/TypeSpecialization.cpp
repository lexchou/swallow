#include "Type.h"
#include "FunctionSymbol.h"
#include "FunctionOverloadedSymbol.h"
#include "GenericArgument.h"
#include "GenericDefinition.h"
#include <cassert>

USE_SWIFT_NS
using namespace std;


static TypePtr specialize(const TypePtr& type, const GenericArgumentPtr& arguments)
{
    //TODO: Fix the void type is nullptr
    if(!type)
        return nullptr;
    if(!type->containsGenericParameters())
        return type;
    switch(type->getCategory())
    {
        case Type::GenericParameter:
        {
            TypePtr ret = arguments->get(type->getName());
            return ret;
        }
        case Type::Function:
        case Type::Closure:
        {
            std::vector<Type::Parameter> params;
            TypePtr returnType = specialize(type->getReturnType(), arguments);
            for(auto param : type->getParameters())
            {
                TypePtr paramType = specialize(param.type, arguments);
                params.push_back(Type::Parameter(param.name, param.inout, paramType));
            }
            //Type::newFunction(<#(vector<Type::Parameter> const &)parameters#>, <#(const TypePtr&)returnType#>, <#(bool)hasVariadicParameters#>, <#(shared_ptr<GenericDefinition> const &)generic#>)
            TypePtr ret = Type::newFunction(params, returnType, type->hasVariadicParameters(), type->getGenericDefinition());
            return ret;
        }
        case Type::Specialized:
        {
            GenericArgumentPtr args(new GenericArgument(type->getGenericArguments()->getDefinition()));
            for (const TypePtr &t : *type->getGenericArguments())
            {
                TypePtr arg = specialize(t, arguments);
                args->add(arg);
            }
            TypePtr ret = Type::newSpecializedType(type->getInnerType(), args);
            return ret;
        }
        default:
            assert(0);
    }

    return nullptr;
}

static FunctionSymbolPtr specialize(const FunctionSymbolPtr& func, const GenericArgumentPtr& arguments)
{
    TypePtr funcType = specialize(func->getType(), arguments);
    FunctionSymbolPtr ret(new FunctionSymbol(func->getName(), funcType, func->getDefinition()));
    return ret;
}


TypePtr Type::newSpecializedType(const TypePtr& innerType, const GenericArgumentPtr& arguments)
{
    Type* ret = new Type(Specialized);
    ret->innerType = innerType;
    ret->genericArguments = arguments;
    //copy members from innerType and update types with given argument
    for(auto entry : innerType->symbols)
    {
        SymbolPtr sym = entry.second;
        if(TypePtr type = dynamic_pointer_cast<Type>(sym))
        {
            sym = specialize(type, arguments);
        }
        else if(FunctionSymbolPtr func = dynamic_pointer_cast<FunctionSymbol>(sym))
        {
            //rebuild the symbol with specialized type
            sym = specialize(func, arguments);
        }
        else if(FunctionOverloadedSymbolPtr funcs = dynamic_pointer_cast<FunctionOverloadedSymbol>(sym))
        {
            FunctionOverloadedSymbolPtr newFuncs(new FunctionOverloadedSymbol());
            for(const FunctionSymbolPtr& func : *funcs)
            {
                FunctionSymbolPtr newFunc = specialize(func, arguments);
                newFuncs->add(newFunc);
            }
            sym = newFuncs;
        }
        else if(SymbolPlaceHolderPtr s = dynamic_pointer_cast<SymbolPlaceHolder>(sym))
        {
            TypePtr t = specialize(s->getType(), arguments);
            SymbolPlaceHolderPtr newSym(new SymbolPlaceHolder(s->getName(), t, s->getRole(), s->flags));
            sym = newSym;
        }
        ret->addMember(entry.first, sym);
    }
    //replace parents and protocols to apply with the generic arguments
    return TypePtr(ret);
}
TypePtr Type::newSpecializedType(const TypePtr& innerType, const TypePtr& argument)
{
    assert(innerType->getGenericDefinition() != nullptr);
    GenericArgumentPtr arguments(new GenericArgument(innerType->getGenericDefinition()));
    arguments->add(argument);

    TypePtr ret = newSpecializedType(innerType, arguments);
    return ret;
}

