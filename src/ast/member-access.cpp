#include <ast/node-visitor.h>
#include "member-access.h"
#include "identifier.h"
USE_SWIFT_NS


MemberAccess::MemberAccess()
    :Expression(NodeType::MemberAccess)
{
}
MemberAccess::~MemberAccess()
{
}
void MemberAccess::setSelf(const ExpressionPtr& self)
{
    this->self = self;
}
void MemberAccess::setField(const IdentifierPtr& field)
{
    this->field = field;
}

ExpressionPtr MemberAccess::getSelf()
{
    return self;
    
}
IdentifierPtr MemberAccess::getField()
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
void MemberAccess::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitMemberAccess);
}
