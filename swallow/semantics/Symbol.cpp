#include "symbol.h"

USE_SWALLOW_NS
using namespace std;

Symbol::Symbol()
:flags(0)
{

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

SymbolPlaceHolder::SymbolPlaceHolder(const std::wstring& name, const TypePtr& type, Role role, int flags)
        :name(name), type(type), role(role)
{
    setFlags(flags);
}