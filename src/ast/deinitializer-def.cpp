
#include "deinitializer-def.h"
#include "code-block.h"
USE_SWIFT_NS


DeinitializerDef::DeinitializerDef()
    :Declaration(1)
{
}
void DeinitializerDef::serialize(std::wostream& out)
{
    out<<L"deinit ";
    getBody()->serialize(out);
}


CodeBlock* DeinitializerDef::getBody()
{
    return static_cast<CodeBlock*>(get(0));
}
void DeinitializerDef::setBody(CodeBlock* body)
{
    set(0, body);
}