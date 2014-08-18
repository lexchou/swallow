#ifndef TYPE_DECLARATION_H
#define TYPE_DECLARATION_H
#include "Declaration.h"
#include <string>

SWIFT_NS_BEGIN

class TypeIdentifier;
typedef std::shared_ptr<class Type> TypePtr;

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

    TypePtr getType();
    void setType(const TypePtr& type);

    std::vector<DeclarationPtr>::iterator begin() {return declarations.begin();}
    std::vector<DeclarationPtr>::iterator end() {return declarations.end();}

public:
    std::vector<TypeIdentifierPtr> parents;
    TypeIdentifierPtr identifier;
    std::vector<DeclarationPtr> declarations;
    TypePtr type;
};

SWIFT_NS_END

#endif//TYPE_DECLARATION_H
