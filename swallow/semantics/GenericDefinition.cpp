#include "GenericDefinition.h"
#include <cassert>
#include <semantics/type.h>

USE_SWIFT_NS
using namespace std;

void GenericDefinition::add(const std::wstring& alias, const TypePtr& type)
{
    auto iter = typeByAlias.find(alias);
    assert(iter == typeByAlias.end());
    typeParameters.push_back(type);
    typeByAlias.insert(make_pair(alias, type));
}
TypePtr GenericDefinition::get(const std::wstring &alias)
{
    auto iter = typeByAlias.find(alias);
    if(iter == typeByAlias.end())
        return nullptr;
    return iter->second;
}
size_t GenericDefinition::numParameters() const
{
    return typeParameters.size();
}
const std::map<std::wstring, TypePtr>& GenericDefinition::getTypeMap() const
{
    return typeByAlias;
}

bool GenericDefinition::equals(const GenericDefinitionPtr& rhs) const
{
    if(rhs == nullptr)
        return false;
    //check type
    if(typeParameters.size() != rhs->typeParameters.size())
        return false;
    auto iter1 = typeParameters.begin();
    auto iter2 = rhs->typeParameters.begin();
    for(; iter1 != typeParameters.end(); iter1++, iter2++)
    {
        if(!Type::equals(*iter1, *iter2))
            return false;
    }
    //check constraint

    return true;
}