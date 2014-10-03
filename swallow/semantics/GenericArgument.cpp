#include "GenericArgument.h"

USE_SWIFT_NS

GenericArgument::GenericArgument(const GenericDefinitionPtr& definition)
:definition(definition)
{

}

void GenericArgument::add(const TypePtr& type)
{
    types.push_back(type);
}

TypePtr GenericArgument::get(size_t index) const
{
    return types[index];
}