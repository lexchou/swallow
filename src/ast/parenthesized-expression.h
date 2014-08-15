#ifndef PARENTHESIZED_EXPRESSION_H
#define PARENTHESIZED_EXPRESSION_H
#include "expression.h"
#include <string>

SWIFT_NS_BEGIN
class ExpressionNode;
class ParenthesizedExpression : public Expression
{
public:

    typedef std::pair<std::wstring, ExpressionPtr> Term;
public:
    ParenthesizedExpression();
    ~ParenthesizedExpression();
public:
    void append(const std::wstring& name, const ExpressionPtr& expr);
    void append(const ExpressionPtr& expr);
    int numExpressions()const;
    std::wstring getName(int idx);
    ExpressionPtr get(int idx);
public:
    virtual void accept(NodeVisitor* visitor);
    std::vector<Term>::iterator begin() {return expressions.begin();}
    std::vector<Term>::iterator end() {return expressions.end();}
public:
    std::vector<Term> expressions;
};

SWIFT_NS_END



#endif//PARENTHESIZED_EXPRESSION_H
