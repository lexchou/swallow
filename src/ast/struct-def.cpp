#include <ast/node-visitor.h>
#include "struct-def.h"
USE_SWIFT_NS


StructDef::StructDef()
    :TypeDeclaration(NodeType::Struct)
{
}

void StructDef::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitStruct);
}