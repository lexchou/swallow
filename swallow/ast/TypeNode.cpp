#include "TypeNode.h"
#include <algorithm>
USE_SWIFT_NS


TypeNode::TypeNode(NodeType::T nodeType)
    :Statement(nodeType)
{
}
TypeNode::~TypeNode()
{

}

void TypeNode::addAttribute(AttributePtr attr)
{
    attributes.push_back(attr);
}
void TypeNode::setAttributes(const std::vector<AttributePtr> attrs)
{
    attributes.clear();
    std::copy(attrs.begin(), attrs.end(), std::back_inserter(attributes));
    
}
const std::vector<AttributePtr>& TypeNode::getAttributes()
{
    return attributes;
}
TypePtr TypeNode::getType()const
{
    return type;
}
void TypeNode::setType(const TypePtr& type)
{
    this->type = type;
}
