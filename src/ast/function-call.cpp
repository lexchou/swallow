#include "function-call.h"
#include "parenthesized-expression.h"
#include "closure.h"
USE_SWIFT_NS


FunctionCall::FunctionCall()
    :Expression(NodeType::FunctionCall), function(NULL), arguments(NULL), trailingClosure(NULL)
{
    
}


FunctionCall::~FunctionCall()
{
    SafeDelete(function);
    SafeDelete(arguments);
    SafeDelete(trailingClosure);
}

void FunctionCall::setFunction(Expression* expr)
{
    function = expr;
}
Expression* FunctionCall::getFunction()
{
    return function;
}
void FunctionCall::setArguments(ParenthesizedExpression* arguments)
{
    this->arguments = arguments;
}
ParenthesizedExpression* FunctionCall::getArguments()
{
    return arguments;
}

void FunctionCall::setTrailingClosure(Closure* trailingClosure)
{
    this->trailingClosure = trailingClosure;
}


Closure* FunctionCall::getTrailingClosure()
{
    return trailingClosure;
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