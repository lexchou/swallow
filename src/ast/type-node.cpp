
#include "type-node.h"
#include "attribute.h"
#include <algorithm>
USE_SWIFT_NS


TypeNode::TypeNode(int numChildren)
    :Statement(numChildren)
{
}
TypeNode::~TypeNode()
{
    std::vector<Attribute*>::iterator iter = attributes.begin();
    for(; iter != attributes.end(); iter++)
    {
        delete (*iter);
    }
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