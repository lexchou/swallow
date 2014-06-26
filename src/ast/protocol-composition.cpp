
#include "protocol-composition.h"
#include "type-identifier.h"
USE_SWIFT_NS


ProtocolComposition::ProtocolComposition()
    :TypeNode(0)
{
}
void ProtocolComposition::serialize(std::wostream& out)
{
    out<<L"protocol<";
    Children::iterator iter = children.begin();
    for(; iter != children.end(); iter++)
    {
        if(iter != children.begin())
            out<<L", ";
        (*iter)->serialize(out);
    }
    out<<L">";
}


void ProtocolComposition::addProtocol(TypeIdentifier* protocol)
{
    children.push_back(protocol);
}
TypeIdentifier* ProtocolComposition::getProtocol(int i)
{
    return static_cast<TypeIdentifier*>(get(i));
}
int ProtocolComposition::numProtocols()
{
    return children.size();
}