#include "function-call.h"
#include "parenthesized-expression.h"
#include "closure-expression.h"
USE_SWIFT_NS


FunctionCall::FunctionCall()
:Expression(3)
{
    
}

void FunctionCall::setFunction(Expression* expr)
{
    set(0, expr);
}
Expression* FunctionCall::getFunction()
{
    return static_cast<Expression*>(get(0));
}
void FunctionCall::setArguments(ParenthesizedExpression* arguments)
{
    set(1, arguments);
}
ParenthesizedExpression* FunctionCall::getArguments()
{
    return static_cast<ParenthesizedExpression*>(get(1));
}

void FunctionCall::setTrailingClosure(ClosureExpression* trailingClosure)
{
    set(2, trailingClosure);
}


ClosureExpression* FunctionCall::getTrailingClosure()
{
    return static_cast<ClosureExpression*>(get(2));
}



void FunctionCall::serialize(std::wostream& out)
{
    getFunction()->serialize(out);
    if(getArguments() == NULL && getTrailingClosure() == NULL)
    {
        out<<L"()";
        return;
    }
    if(getArguments())
        getArguments()->serialize(out);
    if(getTrailingClosure())
    {
        out<<L" ";
        getTrailingClosure()->serialize(out);
    }
}