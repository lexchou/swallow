#ifndef TYPE_IDENTIFIER_H
#define TYPE_IDENTIFIER_H
#include "type-node.h"
#include <string>

SWIFT_NS_BEGIN

class TypeIdentifier : public TypeNode
{
public:
    TypeIdentifier();
    ~TypeIdentifier();
public:
    void setName(const std::wstring& name);
    const std::wstring& getName() const;
    
    void addGenericArgument(TypeNode* argument);
    int numGenericArguments();
    TypeNode* getGenericArgument(int idx);
public:
    virtual void serialize(std::wostream& out);
private:
    std::wstring name;
    std::vector<TypeNode*> genericArguments;
};

SWIFT_NS_END

#endif//TYPE_IDENTIFIER_H
