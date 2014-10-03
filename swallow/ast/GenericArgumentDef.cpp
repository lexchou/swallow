#include "GenericArgumentDef.h"
#include "TypeNode.h"
USE_SWIFT_NS


GenericArgumentDef::GenericArgumentDef()
:Node(NodeType::GenericArgumentDef)
{
}
GenericArgumentDef::~GenericArgumentDef()
{
}

void GenericArgumentDef::addArgument(const TypeNodePtr& type)
{
    arguments.push_back(type);
}
TypeNodePtr GenericArgumentDef::getArgument(int i)
{
    return arguments[i];
}
int GenericArgumentDef::numArguments()
{
    return arguments.size();
}
