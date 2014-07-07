
#include "type-identifier.h"
USE_SWIFT_NS


TypeIdentifier::TypeIdentifier()
{
}
TypeIdentifier::~TypeIdentifier()
{
    SafeDeleteAll(genericArguments);
}
void TypeIdentifier::serialize(std::wostream& out)
{
    out<<name;
    if(numGenericArguments())
    {
        out<<L"<";
        std::vector<TypeNode*>::iterator iter = genericArguments.begin();
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

void TypeIdentifier::addGenericArgument(TypeNode* argument)
{
    genericArguments.push_back(argument);
}
int TypeIdentifier::numGenericArguments()
{
    return genericArguments.size();
}
TypeNode* TypeIdentifier::getGenericArgument(int idx)
{
    return genericArguments[idx];
}