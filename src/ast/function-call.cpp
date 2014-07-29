#include <ast/node-visitor.h>
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

}

void FunctionCall::setFunction(const ExpressionPtr& expr)
{
    function = expr;
}
ExpressionPtr FunctionCall::getFunction()
{
    return function;
}
void FunctionCall::setArguments(const ParenthesizedExpressionPtr& arguments)
{
    this->arguments = arguments;
}
ParenthesizedExpressionPtr FunctionCall::getArguments()
{
    return arguments;
}

void FunctionCall::setTrailingClosure(const ClosurePtr& trailingClosure)
{
    this->trailingClosure = trailingClosure;
}


ClosurePtr FunctionCall::getTrailingClosure()
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
void FunctionCall::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitFunctionCall);
}
