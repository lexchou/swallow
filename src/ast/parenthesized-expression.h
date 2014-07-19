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
    ~ParenthesizedExpression();
public:
    void append(const std::wstring& name, Expression* expr);
    void append(Expression* expr);
    int numExpressions()const;
    std::wstring getName(int idx);
    Expression* get(int idx);
public:
    virtual void serialize(std::wostream& out);
public:
    typedef std::pair<std::wstring, Expression*> Term;
    std::vector<Term> expressions;
};

SWIFT_NS_END



#endif//PARENTHESIZED_EXPRESSION_H
