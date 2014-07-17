
#include "type-node.h"
#include "attribute.h"
#include <algorithm>
USE_SWIFT_NS


TypeNode::TypeNode(NodeType::T nodeType)
    :Statement(nodeType)
{
}
TypeNode::~TypeNode()
{
    SafeDeleteAll(attributes);
}

void TypeNode::addAttribute(Attribute* attr)
{
    attributes.push_back(attr);
}
void TypeNode::setAttributes(const std::vector<Attribute*> attrs)
{
    attributes.clear();
    std::copy(attrs.begin(), attrs.end(), std::back_inserter(attributes));
    
}
const std::vector<Attribute*>& TypeNode::getAttributes()
{
    return attributes;
}