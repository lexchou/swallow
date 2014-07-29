#ifndef TYPE_IDENTIFIER_H
#define TYPE_IDENTIFIER_H
#include "type-node.h"
#include <string>
#include "ast-decl.h"
SWIFT_NS_BEGIN

class TypeIdentifier : public TypeNode
{
public:
    TypeIdentifier();
    ~TypeIdentifier();
public:
    void setName(const std::wstring& name);
    const std::wstring& getName() const;
    
    void addGenericArgument(TypeNodePtr argument);
    int numGenericArguments();
    TypeNodePtr getGenericArgument(int idx);
    
    void setNestedType(TypeIdentifierPtr type);
    TypeIdentifierPtr getNestedType();
public:
    virtual void serialize(std::wostream& out);
private:
    std::wstring name;
    std::vector<TypeNodePtr> genericArguments;
    TypeIdentifierPtr nestedType;
};

SWIFT_NS_END

#endif//TYPE_IDENTIFIER_H
