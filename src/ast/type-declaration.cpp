
#include "type-declaration.h"
#include "type-identifier.h"
USE_SWIFT_NS


TypeDeclaration::TypeDeclaration(NodeType::T nodeType)
    :Declaration(nodeType), identifier(NULL)
{
}


TypeDeclaration::~TypeDeclaration()
{
    SafeDeleteAll(declarations);
    SafeDeleteAll(parents);
    SafeDelete(identifier);
}


void TypeDeclaration::addParent(TypeIdentifier* protocol)
{
    parents.push_back(protocol);
}
int TypeDeclaration::numParents()const
{
    return parents.size();
}
TypeIdentifier* TypeDeclaration::getParent(int i)
{
    if(i < 0 || i >= parents.size())
        return NULL;
    return parents[i];
}



void TypeDeclaration::setIdentifier(TypeIdentifier* id)
{
    this->identifier = id;
}
TypeIdentifier* TypeDeclaration::getIdentifier()
{
    return identifier;
}


void TypeDeclaration::addDeclaration(Declaration* decl)
{
    declarations.push_back(decl);
}
int TypeDeclaration::numDeclarations()const
{
    return declarations.size();
}
Declaration* TypeDeclaration::getDeclaration(int i)
{
    return declarations[i];
}
