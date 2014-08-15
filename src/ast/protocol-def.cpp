
#include "protocol-def.h"
#include "node-visitor.h"
USE_SWIFT_NS


ProtocolDef::ProtocolDef()
    :TypeDeclaration(NodeType::Protocol)
{
}

void ProtocolDef::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitProtocol);
}