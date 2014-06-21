#ifndef IDENTIFIER_H
#define IDENTIFIER_H
#include "expression-node.h"
#include <string>

SWIFT_NS_BEGIN

class Identifier : public ExpressionNode
{
public:
    Identifier(const std::wstring& identifier);
private:
    std::wstring identifier;
};

SWIFT_NS_END



#endif//IDENTIFIER_H
