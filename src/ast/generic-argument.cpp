
#include "generic-argument.h"
#include "type-node.h"
USE_SWIFT_NS


GenericArgument::GenericArgument()
{
}
GenericArgument::~GenericArgument()
{
    SafeDeleteAll(arguments);
}

void GenericArgument::serialize(std::wostream& out)
{
}


void GenericArgument::addArgument(TypeNode* type)
{
    arguments.push_back(type);
}
TypeNode* GenericArgument::getArgument(int i)
{
    return arguments[i];
}
int GenericArgument::numArguments()
{
    return arguments.size();
}
