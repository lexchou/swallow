#ifndef GENERIC_ARGUMENT_H
#define GENERIC_ARGUMENT_H
#include "node.h"
#include <string>

SWIFT_NS_BEGIN

class TypeNode;
class GenericArgument : public Node
{
public:
    GenericArgument();
    ~GenericArgument();
public:
    virtual void serialize(std::wostream& out);
public:
    void addArgument(const TypeNodePtr& type);
    TypeNodePtr getArgument(int i);
    int numArguments();
private:
    std::vector<TypeNodePtr> arguments;
};

SWIFT_NS_END

#endif//GENERIC_ARGUMENT_H
