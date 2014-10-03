#ifndef GENERIC_ARGUMENT_DEF_H
#define GENERIC_ARGUMENT_DEF_H
#include "Node.h"
#include <string>

SWIFT_NS_BEGIN

class TypeNode;
class GenericArgumentDef : public Node
{
public:
    GenericArgumentDef();
    ~GenericArgumentDef();
public:
    void addArgument(const TypeNodePtr& type);
    TypeNodePtr getArgument(int i);
    int numArguments();
private:
    std::vector<TypeNodePtr> arguments;
};

SWIFT_NS_END

#endif//GENERIC_ARGUMENT_DEF_H
