#ifndef IDENTIFIER_H
#define IDENTIFIER_H
#include "expression.h"
#include <string>

SWIFT_NS_BEGIN
class TypeNode;
class GenericArgument;
class Identifier : public Expression
{
public:
    Identifier(const std::wstring& identifier);
    ~Identifier();
public:
    void serialize(std::wostream& out);
    const std::wstring& getIdentifier() const { return identifier;}
    
    void setType(TypeNode* type);
    TypeNode* getType();
    
    GenericArgument* getGenericArgument();
    void setGenericArgument(GenericArgument* val);
private:
    std::wstring identifier;
    TypeNode* type;
    GenericArgument* genericArgument;
};

SWIFT_NS_END



#endif//IDENTIFIER_H
