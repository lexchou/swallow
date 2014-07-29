
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

}

void InitializerDef::serialize(std::wostream& out)
{
}
void InitializerDef::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitInit);
}

void InitializerDef::setConvenience(bool convenience)
{
    this->convenience = convenience;
}
bool InitializerDef::isConvenience()const
{
    return convenience;
}


void InitializerDef::setParameters(const ParametersPtr& parameters)
{
    this->parameters = parameters;
}
ParametersPtr InitializerDef::getParameters()
{
    return parameters;
}

void InitializerDef::setBody(const CodeBlockPtr& body)
{
    this->body = body;
}
CodeBlockPtr InitializerDef::getBody()
{
    return body;
}
