#include "subscript.h"
USE_SWIFT_NS


Subscript::Subscript(Identifier* self, ExpressionNode* index)
    :ExpressionNode(0), self(self), index(index)
{
    
}