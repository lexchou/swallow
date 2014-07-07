#include "member-access.h"
#include "identifier.h"
USE_SWIFT_NS


MemberAccess::MemberAccess(Expression* self, Identifier* field)
{
    setSelf(self);
    setField(field);
}
MemberAccess::~MemberAccess()
{
    SafeDelete(self);
    SafeDelete(field);
}
void MemberAccess::setSelf(Expression* self)
{
    this->self = self;
}
void MemberAccess::setField(Identifier* field)
{
    this->field = field;
}

Expression* MemberAccess::getSelf()
{
    return self;
    
}
Identifier* MemberAccess::getField()
{
    return field;
}


void MemberAccess::serialize(std::wostream& out)
{
    if(getSelf())
        getSelf()->serialize(out);
    out<<L".";
    getSelf()->serialize(out);
}