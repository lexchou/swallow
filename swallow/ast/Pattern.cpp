#include "Pattern.h"
USE_SWIFT_NS


Pattern::Pattern(NodeType::T nodeType)
    :Statement(nodeType)
{
}


void Pattern::setType(const TypePtr& type)
{
    this->type = type;
}
TypePtr Pattern::getType()
{
    return type;
}
