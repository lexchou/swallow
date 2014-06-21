#ifndef LITERAL_NODES_H
#define LITERAL_NODES_H
#include "expression-node.h"
#include <string>

SWIFT_NS_BEGIN

class BooleanLiteral : public ExpressionNode
{
public:
    BooleanLiteral(bool val);
private:
    bool value;
};
class IntegerLiteral : public ExpressionNode
{
public:
    IntegerLiteral(const std::wstring& val);
private:
    std::wstring value;
};
class FloatLiteral : public ExpressionNode
{
public:
    FloatLiteral(const std::wstring& val);
private:
    std::wstring value;
};
class StringLiteral : public ExpressionNode
{
public:
    StringLiteral(const std::wstring& val);
private:
    std::wstring value;

};

SWIFT_NS_END



#endif//LITERAL_NODES_H
