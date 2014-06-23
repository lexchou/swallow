#ifndef FUNCTION_CALL_H
#define FUNCTION_CALL_H
#include "expression-node.h"

SWIFT_NS_BEGIN

class ParenthesizedExpression;
class ClosureExpression;
class FunctionCall : public ExpressionNode
{
public:
    FunctionCall();
public:
    
    void setFunction(ExpressionNode* expr);
    ExpressionNode* getFunction();
    
    void setArguments(ParenthesizedExpression* arguments);
    ParenthesizedExpression* getArguments();
    
    void setTrailingClosure(ClosureExpression* trailingClosure);
    ClosureExpression* getTrailingClosure();
public:
    virtual void serialize(std::wostream& out);
};

SWIFT_NS_END

#endif//FUNCTION_CALL_H