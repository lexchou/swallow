#ifndef GENERIC_ARGUMENT_H
#define GENERIC_ARGUMENT_H
#include "Node.h"
#include <string>

SWIFT_NS_BEGIN

class TypeNode;
class GenericArgument : public Node
{
public:
    GenericArgument();
    ~GenericArgument();
public:
    void addArgument(const TypeNodePtr& type);
    TypeNodePtr getArgument(int i);
    int numArguments();
private:
    std::vector<TypeNodePtr> arguments;
};

SWIFT_NS_END

#endif//GENERIC_ARGUMENT_H
