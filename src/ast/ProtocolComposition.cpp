#include "ProtocolComposition.h"
#include "TypeIdentifier.h"
USE_SWIFT_NS


ProtocolComposition::ProtocolComposition()
    :TypeNode(NodeType::ProtocolComposition)
{
}

ProtocolComposition::~ProtocolComposition()
{

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
