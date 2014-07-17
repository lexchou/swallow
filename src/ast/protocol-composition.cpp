
#include "protocol-composition.h"
#include "type-identifier.h"
USE_SWIFT_NS


ProtocolComposition::ProtocolComposition()
    :TypeNode(NodeType::ProtocolComposition)
{
}

ProtocolComposition::~ProtocolComposition()
{
    SafeDeleteAll(protocols);
}

void ProtocolComposition::serialize(std::wostream& out)
{
    out<<L"protocol<";
    std::vector<TypeIdentifier*>::iterator iter = protocols.begin();
    for(; iter != protocols.end(); iter++)
    {
        if(iter != protocols.begin())
            out<<L", ";
        (*iter)->serialize(out);
    }
    out<<L">";
}


void ProtocolComposition::addProtocol(TypeIdentifier* protocol)
{
    protocols.push_back(protocol);
}
TypeIdentifier* ProtocolComposition::getProtocol(int i)
{
    return protocols[i];
}
int ProtocolComposition::numProtocols()
{
    return protocols.size();
}