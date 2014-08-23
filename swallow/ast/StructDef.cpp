#include "StructDef.h"
#include "NodeVisitor.h"
USE_SWIFT_NS


StructDef::StructDef()
    :TypeDeclaration(NodeType::Struct)
{
}

void StructDef::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitStruct);
}
