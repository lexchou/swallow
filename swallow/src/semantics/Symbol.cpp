#include "semantics/Symbol.h"

USE_SWALLOW_NS
using namespace std;

Symbol::Symbol(SymbolKind kind)
:flags(0), kind(kind)
{
    accessLevel = AccessLevelUndefined;
}

void Symbol::setFlags(int flags)
{
    this->flags = flags;
}
int Symbol::getFlags()const
{
    return flags;
}

bool Symbol::hasFlags(SymbolFlags flags)const
{
    return (this->flags & flags) == flags;
}
void Symbol::setFlags(SymbolFlags flags, bool set)
{
    if(set)
        this->flags |= flags;
    else
        this->flags &= ~flags;
}

/*!
 * If this symbol is defined as a member of a type, returns the type that declared this symbol.
 */
TypePtr Symbol::getDeclaringType() const
{
    return declaringType;
}

Module::Module(const std::wstring& name, const TypePtr& type)
:Symbol(SymbolKindModule), name(name), type(type)
{

}

SymbolPtr Module::getSymbol(const std::wstring& name)
{
    auto iter = symbols.find(name);
    if(iter != symbols.end())
        return iter->second;
    return nullptr;
}
void Module::addSymbol(const std::wstring& name, const SymbolPtr& symbol)
{
    symbols.insert(make_pair(name, symbol));
}


SymbolPlaceHolder::SymbolPlaceHolder(const std::wstring& name, const TypePtr& type, Role role, int flags)
    :Symbol(SymbolKindPlaceholder), name(name), type(type), role(role)
{
    setFlags(flags);
}
ComputedPropertySymbol::ComputedPropertySymbol(const std::wstring &name, const TypePtr &type, int flags)
    :Symbol(SymbolKindComputedProperty), name(name), type(type)
{
    setFlags(flags);
}
