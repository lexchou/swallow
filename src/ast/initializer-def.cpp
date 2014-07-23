
#include "initializer-def.h"
#include "parameters.h"
#include "code-block.h"
#include "node-visitor.h"
USE_SWIFT_NS


InitializerDef::InitializerDef()
    :Declaration(NodeType::Init), convenience(false), parameters(NULL), body(NULL)
{
}

InitializerDef::~InitializerDef()
{
    SafeDelete(body);
    SafeDelete(parameters);
}

void InitializerDef::serialize(std::wostream& out)
{
}
void InitializerDef::accept(NodeVisitor* visitor)
{
    visitor->visitInit(this);
}

void InitializerDef::setConvenience(bool convenience)
{
    this->convenience = convenience;
}
bool InitializerDef::isConvenience()const
{
    return convenience;
}


void InitializerDef::setParameters(Parameters* parameters)
{
    this->parameters = parameters;
}
Parameters* InitializerDef::getParameters()
{
    return parameters;
}

void InitializerDef::setBody(CodeBlock* body)
{
    this->body = body;
}
CodeBlock* InitializerDef::getBody()
{
    return body;
}
