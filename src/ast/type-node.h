#ifndef TYPE_NODE_H
#define TYPE_NODE_H
#include "statement.h"
#include <string>
#include <vector>

SWIFT_NS_BEGIN
class Attribute;
class TypeNode : public Statement
{
public:
    TypeNode(int numChildren);
    ~TypeNode();
public:
    void addAttribute(Attribute* attr);
    void setAttributes(const std::vector<Attribute*> attrs);
    const std::vector<Attribute*>& getAttributes();
private:
    std::vector<Attribute*> attributes;
};

SWIFT_NS_END

#endif//TYPE_NODE_H
