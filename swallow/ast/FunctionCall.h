#ifndef FUNCTION_CALL_H
#define FUNCTION_CALL_H
#include "Expression.h"

SWIFT_NS_BEGIN

class ParenthesizedExpression;
class Closure;
class FunctionCall : public Expression
{
public:
    FunctionCall();
    ~FunctionCall();
public:
    
    void setFunction(const ExpressionPtr& expr);
    ExpressionPtr getFunction();
    
    void setArguments(const ParenthesizedExpressionPtr& arguments);
    ParenthesizedExpressionPtr getArguments();
    
    void setTrailingClosure(const ClosurePtr& trailingClosure);
    ClosurePtr getTrailingClosure();
public:
    virtual void accept(NodeVisitor* visitor);
private:
    ExpressionPtr function;
    ParenthesizedExpressionPtr arguments;
    ClosurePtr trailingClosure;
};

SWIFT_NS_END

#endif//FUNCTION_CALL_H
