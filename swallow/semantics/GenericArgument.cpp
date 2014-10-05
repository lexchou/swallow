#include "GenericArgument.h"
#include "GenericDefinition.h"

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
TypePtr GenericArgument::get(const std::wstring& name) const
{
    auto iter = definition->constraints.find(name);
    if(iter == definition->constraints.end())
        return nullptr;
    TypePtr ret = get(iter->second->index);
    return ret;
}
GenericDefinitionPtr GenericArgument::getDefinition() const
{
    return definition;
}