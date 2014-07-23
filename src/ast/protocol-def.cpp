
#include "protocol-def.h"
#include "node-visitor.h"
USE_SWIFT_NS


ProtocolDef::ProtocolDef()
    :TypeDeclaration(NodeType::Protocol)
{
}
void ProtocolDef::serialize(std::wostream& out)
{
}
void ProtocolDef::accept(NodeVisitor* visitor)
{
    visitor->visitProtocol(this);
}