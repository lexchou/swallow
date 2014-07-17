
#include "deinitializer-def.h"
#include "code-block.h"
USE_SWIFT_NS


DeinitializerDef::DeinitializerDef()
    :Declaration(NodeType::Deinit), body(NULL)
{
}
DeinitializerDef::~DeinitializerDef()
{
    SafeDelete(body);
}

void DeinitializerDef::serialize(std::wostream& out)
{
    out<<L"deinit ";
    getBody()->serialize(out);
}


CodeBlock* DeinitializerDef::getBody()
{
    return body;
}
void DeinitializerDef::setBody(CodeBlock* body)
{
    this->body = body;
}