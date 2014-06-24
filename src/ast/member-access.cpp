#include "member-access.h"
#include "identifier.h"
USE_SWIFT_NS


MemberAccess::MemberAccess(ExpressionNode* self, Identifier* field)
    :ExpressionNode(2)
{
    setSelf(self);
    setField(field);
}

void MemberAccess::setSelf(ExpressionNode* self)
{
    set(0, self);
}
void MemberAccess::setField(Identifier* field)
{
    set(1, field);
}

ExpressionNode* MemberAccess::getSelf()
{
    return static_cast<ExpressionNode*>(get(0));
    
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