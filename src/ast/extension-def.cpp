
#include "extension-def.h"
#include "type-identifier.h"
USE_SWIFT_NS


ExtensionDef::ExtensionDef()
    :Declaration(1)
{
}
ExtensionDef::~ExtensionDef()
{
    Children::iterator iter = children.begin();
    for(; iter != children.end(); iter++)
    {
        delete *iter;
    }
}
void ExtensionDef::serialize(std::wostream& out)
{
}



void ExtensionDef::addAdoptedProtocol(TypeIdentifier* protocol)
{
    adoptedProtocols.push_back(protocol);
}
int ExtensionDef::numAdoptedProtocols()
{
    return adoptedProtocols.size();
}
TypeIdentifier* ExtensionDef::getAdoptedProtocol(int i)
{
    if(i < 0 || i >= adoptedProtocols.size())
        return NULL;
    return adoptedProtocols[i];
}



void ExtensionDef::setIdentifier(TypeIdentifier* id)
{
    set(0, id);
}
TypeIdentifier* ExtensionDef::getIdentifier()
{
    return static_cast<TypeIdentifier*>(get(0));
}


void ExtensionDef::addDeclaration(Declaration* decl)
{
    children.push_back(decl);
}
int ExtensionDef::numDeclarations()
{
    return children.size() - 1;
}
Declaration* ExtensionDef::getDeclaration(int i)
{
    return static_cast<Declaration*>(get(i + 1));
}
