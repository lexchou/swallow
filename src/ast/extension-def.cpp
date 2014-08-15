#include <ast/node-visitor.h>
#include "extension-def.h"
USE_SWIFT_NS


ExtensionDef::ExtensionDef()
    :TypeDeclaration(NodeType::Extension)
{
}

void ExtensionDef::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitExtension);
}
