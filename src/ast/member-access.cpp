#include "member-access.h"
#include "identifier.h"
USE_SWIFT_NS


MemberAccess::MemberAccess(Expression* self, Identifier* field)
    :Expression(2)
{
    setSelf(self);
    setField(field);
}

void MemberAccess::setSelf(Expression* self)
{
    set(0, self);
}
void MemberAccess::setField(Identifier* field)
{
    set(1, field);
}

Expression* MemberAccess::getSelf()
{
    return static_cast<Expression*>(get(0));
    
}
Identifier* MemberAccess::getField()
{
    return static_cast<Identifier*>(get(1));
}


void MemberAccess::serialize(std::wostream& out)
{
    if(getSelf())
        getSelf()->serialize(out);
    out<<L".";
    getSelf()->serialize(out);
}