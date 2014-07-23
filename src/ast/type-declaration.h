#ifndef TYPE_DECLARATION_H
#define TYPE_DECLARATION_H
#include "declaration.h"
#include <string>

SWIFT_NS_BEGIN
class TypeIdentifier;

class TypeDeclaration : public Declaration
{
public:
    TypeDeclaration(NodeType::T nodeType);
    ~TypeDeclaration();
public:
    void addParent(TypeIdentifier* parent);
    int numParents()const;
    TypeIdentifier* getParent(int i);
    const std::vector<TypeIdentifier*>& getParents() {return parents;}
    
    void setIdentifier(TypeIdentifier* id);
    TypeIdentifier* getIdentifier();
    
    void addDeclaration(Declaration* decl);
    int numDeclarations()const;
    Declaration* getDeclaration(int i);

    std::vector<Declaration*>::iterator begin() {return declarations.begin();}
    std::vector<Declaration*>::iterator end() {return declarations.end();}

public:
    std::vector<TypeIdentifier*> parents;
    TypeIdentifier* identifier;
    std::vector<Declaration*> declarations;
};

SWIFT_NS_END

#endif//TYPE_DECLARATION_H
