
#include "initializer-def.h"
#include "parameters.h"
#include "code-block.h"
USE_SWIFT_NS


InitializerDef::InitializerDef()
    :Declaration(2), convenience(false)
{
}
void InitializerDef::serialize(std::wostream& out)
{
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
    set(0, parameters);
}
Parameters* InitializerDef::getParameters()
{
    return static_cast<Parameters*>(get(0));
}

void InitializerDef::setBody(CodeBlock* body)
{
    set(1, body);
}
CodeBlock* InitializerDef::getBody()
{
    return static_cast<CodeBlock*>(get(1));
}
