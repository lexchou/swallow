#ifndef PARENTHESIZED_EXPRESSION_H
#define PARENTHESIZED_EXPRESSION_H
#include "expression-node.h"
#include <string>

SWIFT_NS_BEGIN
class ExpressionNode;
class ParenthesizedExpression : public ExpressionNode
{
public:
    ParenthesizedExpression();
public:
    void append(const std::wstring& name, ExpressionNode* expr);
    void append(ExpressionNode* expr);
public:
    virtual void serialize(std::wostream& out);
private:
    std::vector<std::wstring> names;
};

SWIFT_NS_END



#endif//PARENTHESIZED_EXPRESSION_H
