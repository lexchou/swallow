#include "semantics/Symbol.h"

USE_SWALLOW_NS
using namespace std;

Symbol::Symbol()
:flags(0)
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



SymbolPlaceHolder::SymbolPlaceHolder(const std::wstring& name, const TypePtr& type, Role role, int flags)
        :name(name), type(type), role(role)
{
    setFlags(flags);
}
