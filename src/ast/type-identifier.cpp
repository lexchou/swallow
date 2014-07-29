
#include "type-identifier.h"
USE_SWIFT_NS


TypeIdentifier::TypeIdentifier()
    :TypeNode(NodeType::TypeIdentifier)
{
    nestedType = NULL;
}
TypeIdentifier::~TypeIdentifier()
{
}
void TypeIdentifier::serialize(std::wostream& out)
{
    out<<name;
    if(numGenericArguments())
    {
        out<<L"<";
        std::vector<TypeNodePtr>::iterator iter = genericArguments.begin();
        for(; iter != genericArguments.end(); iter++)
        {
            if(iter != genericArguments.begin())
                out<<L", ";
            (*iter)->serialize(out);
        }
        out<<L">";
    }
}


void TypeIdentifier::setName(const std::wstring& name)
{
    this->name = name;
}
const std::wstring& TypeIdentifier::getName() const
{
    return name;
}

void TypeIdentifier::addGenericArgument(TypeNodePtr argument)
{
    genericArguments.push_back(argument);
}
int TypeIdentifier::numGenericArguments()
{
    return genericArguments.size();
}
TypeNodePtr TypeIdentifier::getGenericArgument(int idx)
{
    return genericArguments[idx];
}


void TypeIdentifier::setNestedType(TypeIdentifierPtr type)
{
    this->nestedType = type;
}
TypeIdentifierPtr TypeIdentifier::getNestedType()
{
    return nestedType;
}