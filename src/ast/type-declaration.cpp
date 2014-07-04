
#include "type-declaration.h"
#include "type-identifier.h"
USE_SWIFT_NS


TypeDeclaration::TypeDeclaration()
    :Declaration(1)
{
}


TypeDeclaration::~TypeDeclaration()
{
    std::vector<TypeIdentifier*>::iterator iter = parents.begin();
    for(; iter != parents.end(); iter++)
    {
        delete *iter;
    }

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
    set(0, id);
}
TypeIdentifier* TypeDeclaration::getIdentifier()
{
    return static_cast<TypeIdentifier*>(get(0));
}


void TypeDeclaration::addDeclaration(Declaration* decl)
{
    children.push_back(decl);
}
int TypeDeclaration::numDeclarations()const
{
    return children.size() - 1;
}
Declaration* TypeDeclaration::getDeclaration(int i)
{
    return static_cast<Declaration*>(get(i + 1));
}
