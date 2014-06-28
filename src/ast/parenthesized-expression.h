#ifndef PARENTHESIZED_EXPRESSION_H
#define PARENTHESIZED_EXPRESSION_H
#include "expression.h"
#include <string>

SWIFT_NS_BEGIN
class ExpressionNode;
class ParenthesizedExpression : public Expression
{
public:
    ParenthesizedExpression();
public:
    void append(const std::wstring& name, Expression* expr);
    void append(Expression* expr);
    std::wstring getName(int idx);
public:
    virtual void serialize(std::wostream& out);
private:
    std::vector<std::wstring> names;
};

SWIFT_NS_END



#endif//PARENTHESIZED_EXPRESSION_H
