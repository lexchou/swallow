#ifndef IDENTIFIER_H
#define IDENTIFIER_H
#include "expression-node.h"
#include <string>

SWIFT_NS_BEGIN

class Identifier : public ExpressionNode
{
public:
    Identifier(const std::wstring& identifier);
public:
    void serialize(std::wostream& out);
    const std::wstring& getIdentifier() const { return identifier;}
private:
    std::wstring identifier;
};

SWIFT_NS_END



#endif//IDENTIFIER_H
