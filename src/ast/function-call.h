#ifndef FUNCTION_CALL_H
#define FUNCTION_CALL_H
#include "expression.h"

SWIFT_NS_BEGIN

class ParenthesizedExpression;
class ClosureExpression;
class FunctionCall : public Expression
{
public:
    FunctionCall();
    ~FunctionCall();
public:
    
    void setFunction(Expression* expr);
    Expression* getFunction();
    
    void setArguments(ParenthesizedExpression* arguments);
    ParenthesizedExpression* getArguments();
    
    void setTrailingClosure(ClosureExpression* trailingClosure);
    ClosureExpression* getTrailingClosure();
public:
    virtual void serialize(std::wostream& out);
private:
    Expression* function;
    ParenthesizedExpression* arguments;
    ClosureExpression* trailingClosure;
};

SWIFT_NS_END

#endif//FUNCTION_CALL_H