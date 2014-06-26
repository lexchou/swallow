
#include "type-identifier.h"
USE_SWIFT_NS


TypeIdentifier::TypeIdentifier()
    :TypeNode(0)
{
}
void TypeIdentifier::serialize(std::wostream& out)
{
    out<<name;
    if(numGenericArguments())
    {
        out<<L"<";
        Children::iterator iter = children.begin();
        for(; iter != children.end(); iter++)
        {
            if(iter != children.begin())
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
    children.push_back(argument);
}
int TypeIdentifier::numGenericArguments()
{
    return children.size();
}
TypeNode* TypeIdentifier::getGenericArgument(int idx)
{
    return static_cast<TypeNode*>(get(idx));
}