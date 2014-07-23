#include <ast/node-visitor.h>
#include "extension-def.h"
#include "type-identifier.h"
USE_SWIFT_NS


ExtensionDef::ExtensionDef()
    :TypeDeclaration(NodeType::Extension)
{
}
void ExtensionDef::serialize(std::wostream& out)
{
}
void ExtensionDef::accept(NodeVisitor* visitor)
{
    visitor->visitExtension(this);
}
