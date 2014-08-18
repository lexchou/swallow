#ifndef TYPE_NODE_H
#define TYPE_NODE_H
#include "Statement.h"
#include <string>
#include <vector>

SWIFT_NS_BEGIN
class Attribute;
class TypeNode : public Statement
{
public:
    TypeNode(NodeType::T nodeType);
    ~TypeNode();
public:
    void addAttribute(AttributePtr attr);
    void setAttributes(const std::vector<AttributePtr> attrs);
    const std::vector<AttributePtr>& getAttributes();
private:
    std::vector<AttributePtr> attributes;
};


SWIFT_NS_END

#endif//TYPE_NODE_H
