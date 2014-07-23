#include <ast/node-visitor.h>
#include "struct-def.h"
USE_SWIFT_NS


StructDef::StructDef()
    :TypeDeclaration(NodeType::Struct)
{
}
void StructDef::serialize(std::wostream& out)
{
}

void StructDef::accept(NodeVisitor* visitor)
{
    visitor->visitStruct(this);
}