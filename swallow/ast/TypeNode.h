#ifndef TYPE_NODE_H
#define TYPE_NODE_H
#include "Statement.h"
#include <string>
#include <vector>

SWIFT_NS_BEGIN
class Attribute;
typedef std::shared_ptr<class Type> TypePtr;
class TypeNode : public Statement
{
public:
    TypeNode(NodeType::T nodeType);
    ~TypeNode();
public:
    void addAttribute(AttributePtr attr);
    void setAttributes(const std::vector<AttributePtr> attrs);
    const std::vector<AttributePtr>& getAttributes();

    TypePtr getType()const;
    void setType(const TypePtr& type);
private:
    std::vector<AttributePtr> attributes;
    TypePtr type;
};


SWIFT_NS_END

#endif//TYPE_NODE_H
