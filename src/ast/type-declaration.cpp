
#include "type-declaration.h"
#include "type-identifier.h"
USE_SWIFT_NS


TypeDeclaration::TypeDeclaration(NodeType::T nodeType)
    :Declaration(nodeType), identifier(NULL)
{
}


TypeDeclaration::~TypeDeclaration()
{

}


void TypeDeclaration::addParent(const TypeIdentifierPtr& protocol)
{
    parents.push_back(protocol);
}
int TypeDeclaration::numParents()const
{
    return parents.size();
}
TypeIdentifierPtr TypeDeclaration::getParent(int i)
{
    if(i < 0 || i >= parents.size())
        return NULL;
    return parents[i];
}



void TypeDeclaration::setIdentifier(const TypeIdentifierPtr& id)
{
    this->identifier = id;
}
TypeIdentifierPtr TypeDeclaration::getIdentifier()
{
    return identifier;
}


void TypeDeclaration::addDeclaration(const DeclarationPtr& decl)
{
    declarations.push_back(decl);
}
int TypeDeclaration::numDeclarations()const
{
    return declarations.size();
}
DeclarationPtr TypeDeclaration::getDeclaration(int i)
{
    return declarations[i];
}

TypePtr TypeDeclaration::getType()
{
    return type;
}
void TypeDeclaration::setType(const TypePtr& type)
{
    this->type = type;
}