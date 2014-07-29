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
    void addParent(const TypeIdentifierPtr& parent);
    int numParents()const;
    TypeIdentifierPtr getParent(int i);
    const std::vector<TypeIdentifierPtr>& getParents() {return parents;}
    
    void setIdentifier(const TypeIdentifierPtr& id);
    TypeIdentifierPtr getIdentifier();
    
    void addDeclaration(const DeclarationPtr& decl);
    int numDeclarations()const;
    DeclarationPtr getDeclaration(int i);

    std::vector<DeclarationPtr>::iterator begin() {return declarations.begin();}
    std::vector<DeclarationPtr>::iterator end() {return declarations.end();}

public:
    std::vector<TypeIdentifierPtr> parents;
    TypeIdentifierPtr identifier;
    std::vector<DeclarationPtr> declarations;
};

SWIFT_NS_END

#endif//TYPE_DECLARATION_H
