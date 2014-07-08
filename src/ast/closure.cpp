
#include "closure.h"
#include "parameters.h"
#include "type-node.h"
USE_SWIFT_NS


Closure::Closure()
:captureSpecifier(_), capture(NULL), parameters(NULL), returnType(NULL)
{
}

Closure::~Closure()
{
    SafeDelete(capture);
    SafeDelete(parameters);
    SafeDelete(returnType);
    SafeDeleteAll(statements);
}

void Closure::serialize(std::wostream& out)
{
}


Closure::CaptureSpecifier Closure::getCaptureSpecifier()const
{
    return captureSpecifier;
}
void Closure::setCaptureSpecifier(CaptureSpecifier val)
{
    captureSpecifier = val;
}

void Closure::setCapture(Expression* capture)
{
    this->capture = capture;
}
Expression* Closure::getCapture()
{
    return capture;
}

void Closure::setParameters(Parameters* val)
{
    parameters = val;
}
Parameters* Closure::getParameters()
{
    return parameters;
}

void Closure::setReturnType(TypeNode* val)
{
    returnType = val;
}
TypeNode* Closure::getReturnType()
{
    return returnType;
}

void Closure::addStatement(Statement* st)
{
    statements.push_back(st);
}
int Closure::numStatement()const
{
    return statements.size();
}
Statement* Closure::getStatement(int i)
{
    return statements[i];
}