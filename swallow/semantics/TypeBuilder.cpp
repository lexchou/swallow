#include "TypeBuilder.h"
#include "FunctionSymbol.h"
#include "FunctionOverloadedSymbol.h"
#include <cassert>


USE_SWIFT_NS

using namespace std;

TypeBuilder::TypeBuilder(Category category)
:Type(category)
{
}
void TypeBuilder::setInitializer(const FunctionOverloadedSymbolPtr& initializer)
{
    this->initializer = initializer;
}

void TypeBuilder::addParameter(const Type::Parameter& param)
{
    parameters.push_back(param);
}

void TypeBuilder::setParentType(const TypePtr &type)
{
    this->parentType = type;
}
void TypeBuilder::setInnerType(const TypePtr &type)
{
    innerType = type;
}
void TypeBuilder::addProtocol(const TypePtr &protocol)
{
    protocols.push_back(protocol);
    auto iter = parents.find(protocol);
    if(iter == parents.end())
        parents.insert(make_pair(protocol, 1));
    else
        iter->second = 1;
}
void TypeBuilder::addParentTypesFrom(const TypePtr& type)
{
    for(auto parent : type->parents)
    {
        addParentType(parent.first, parent.second + 1);
    }
    addParentType(type, 1);
}
void TypeBuilder::addParentType(const TypePtr& type, int distance)
{
    auto iter = parents.find(type);
    if(iter == parents.end())
    {
        parents.insert(std::make_pair(type, distance));
    }
    else if(iter->second > distance)
    {
        iter->second = distance;
    }
}

void TypeBuilder::addMember(const SymbolPtr& symbol)
{
    addMember(symbol->getName(), symbol);
}

void TypeBuilder::addMember(const std::wstring& name, const SymbolPtr& member)
{
    if(SymbolPlaceHolderPtr s = dynamic_pointer_cast<SymbolPlaceHolder>(member))
    {
        members.insert(std::make_pair(name, member));
        if(s->getRole() == SymbolPlaceHolder::R_PROPERTY)
        {
            //computed property
            computedProperties.push_back(s);
        }
        else
        {
            //stored property
            storedProperties.push_back(s);
        }
        return;
    }
    else if(TypePtr type = dynamic_pointer_cast<Type>(member))
    {
        members.insert(std::make_pair(name, member));
        //associated type
        associatedTypes.insert(make_pair(name, type));
        return;
    }
    auto iter = members.find(name);
    if(iter != members.end())
    {
        FunctionOverloadedSymbolPtr oldFuncs = static_pointer_cast<FunctionOverloadedSymbol>(iter->second);
        if(FunctionSymbolPtr func = dynamic_pointer_cast<FunctionSymbol>(member))
            oldFuncs->add(func);
        else if(FunctionOverloadedSymbolPtr funcs = dynamic_pointer_cast<FunctionOverloadedSymbol>(member))
            oldFuncs->add(funcs);
        else
            assert(0 && "Unknown member type");
    }
    else
    {
        if(FunctionSymbolPtr func = dynamic_pointer_cast<FunctionSymbol>(member))
        {
            FunctionOverloadedSymbolPtr funcs(new FunctionOverloadedSymbol(name));
            funcs->add(func);
            members.insert(make_pair(name, funcs));
            functions.push_back(funcs);
        }
        else if(FunctionOverloadedSymbolPtr funcs = dynamic_pointer_cast<FunctionOverloadedSymbol>(member))
        {
            members.insert(make_pair(name, funcs));
            functions.push_back(funcs);
        }
        else
        {
            member->getName();
            assert(0 && "Unknown member type");
        }
    }
}


