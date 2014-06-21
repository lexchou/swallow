#include "member-access.h"
USE_SWIFT_NS


MemberAccess::MemberAccess(Identifier* self, Identifier* field)
    :ExpressionNode(0), self(self), field(field)
{
    
}