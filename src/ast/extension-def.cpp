
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

