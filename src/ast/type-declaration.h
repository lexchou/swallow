#ifndef TYPE_DECLARATION_H
#define TYPE_DECLARATION_H
#include "declaration.h"
#include <string>

SWIFT_NS_BEGIN
class TypeIdentifier;

class TypeDeclaration : public Declaration
{
public:
    TypeDeclaration();
    ~TypeDeclaration();
public:
    void addParent(TypeIdentifier* parent);
    int numParents()const;
    TypeIdentifier* getParent(int i);
    
    void setIdentifier(TypeIdentifier* id);
    TypeIdentifier* getIdentifier();
    
    void addDeclaration(Declaration* decl);
    int numDeclarations()const;
    Declaration* getDeclaration(int i);
private:
    std::vector<TypeIdentifier*> parents;
    TypeIdentifier* identifier;
    std::vector<Declaration*> declarations;
};

SWIFT_NS_END

#endif//TYPE_DECLARATION_H
