
#include "protocol-composition.h"
#include "type-identifier.h"
USE_SWIFT_NS


ProtocolComposition::ProtocolComposition()
    :TypeNode(NodeType::ProtocolComposition)
{
}

ProtocolComposition::~ProtocolComposition()
{

}

void ProtocolComposition::serialize(std::wostream& out)
{
    out<<L"protocol<";
    for(const TypeIdentifierPtr& protocol : protocols)
    {
        protocol->serialize(out);
    }
    out<<L">";
}


void ProtocolComposition::addProtocol(TypeIdentifierPtr protocol)
{
    protocols.push_back(protocol);
}
TypeIdentifierPtr ProtocolComposition::getProtocol(int i)
{
    return protocols[i];
}
int ProtocolComposition::numProtocols()
{
    return protocols.size();
}