
#include "attribute.h"
USE_SWIFT_NS


Attribute::Attribute()
    :Node(0)
{
}
void Attribute::serialize(std::wostream& out)
{
    out<<L"@"<<name;
}
void Attribute::setName(const std::wstring& name)
{
    this->name = name;
}
const std::wstring& Attribute::getName()const
{
    return name;
}
void Attribute::addBalancedToken(const std::wstring& token)
{
    balancedTokens.push_back(token);
}




void Attribute::disposeAll(std::vector<Attribute*>& attributes)
{
    std::vector<Attribute*>::iterator iter = attributes.begin();
    for(; iter != attributes.end(); iter++)
    {
        delete *iter;
    }
    attributes.clear();
}