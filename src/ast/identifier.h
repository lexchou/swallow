#ifndef IDENTIFIER_H
#define IDENTIFIER_H
#include "expression.h"
#include <string>

SWIFT_NS_BEGIN
class TypeNode;
class Identifier : public Expression
{
public:
    Identifier(const std::wstring& identifier);
public:
    void serialize(std::wostream& out);
    const std::wstring& getIdentifier() const { return identifier;}
    
    void setType(TypeNode* type);
    TypeNode* getType();
private:
    std::wstring identifier;
};

SWIFT_NS_END



#endif//IDENTIFIER_H
