#include "closure.h"
#include <ast/node-visitor.h>
USE_SWIFT_NS


Closure::Closure()
    :Expression(NodeType::Closure), captureSpecifier(_), capture(NULL), parameters(NULL), returnType(NULL)
{
}

Closure::~Closure()
{

}


void Closure::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitClosure);
}

Closure::CaptureSpecifier Closure::getCaptureSpecifier()const
{
    return captureSpecifier;
}
void Closure::setCaptureSpecifier(CaptureSpecifier val)
{
    captureSpecifier = val;
}

void Closure::setCapture(const ExpressionPtr& capture)
{
    this->capture = capture;
}
ExpressionPtr Closure::getCapture()
{
    return capture;
}

void Closure::setParameters(const ParametersPtr& val)
{
    parameters = val;
}
ParametersPtr Closure::getParameters()
{
    return parameters;
}

void Closure::setReturnType(const TypeNodePtr& val)
{
    returnType = val;
}
TypeNodePtr Closure::getReturnType()
{
    return returnType;
}

void Closure::addStatement(const StatementPtr& st)
{
    statements.push_back(st);
}
int Closure::numStatement()const
{
    return statements.size();
}
StatementPtr Closure::getStatement(int i)
{
    return statements[i];
}