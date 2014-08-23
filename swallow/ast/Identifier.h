#ifndef IDENTIFIER_H
#define IDENTIFIER_H
#include "Expression.h"
#include <string>

SWIFT_NS_BEGIN
class TypeNode;
class GenericArgument;
class Type;
class Identifier : public Expression
{
public:
    Identifier();
    ~Identifier();
public:
    virtual void accept(NodeVisitor* visitor);
public:
    const std::wstring& getIdentifier() const { return identifier;}
    void setIdentifier(const std::wstring& id){identifier = id;}
    
    void setDeclaredType(const TypeNodePtr& type);
    TypeNodePtr getDeclaredType();
    
    GenericArgumentPtr getGenericArgument();
    void setGenericArgument(const GenericArgumentPtr& val);
protected:
    std::wstring identifier;
    TypeNodePtr declaredType;
    GenericArgumentPtr genericArgument;
};

SWIFT_NS_END



#endif//IDENTIFIER_H
