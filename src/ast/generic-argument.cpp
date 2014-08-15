
#include "generic-argument.h"
#include "type-node.h"
USE_SWIFT_NS


GenericArgument::GenericArgument()
:Node(NodeType::GenericArgument)
{
}
GenericArgument::~GenericArgument()
{
}

void GenericArgument::addArgument(const TypeNodePtr& type)
{
    arguments.push_back(type);
}
TypeNodePtr GenericArgument::getArgument(int i)
{
    return arguments[i];
}
int GenericArgument::numArguments()
{
    return arguments.size();
}
