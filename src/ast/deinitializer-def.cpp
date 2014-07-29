
#include "deinitializer-def.h"
#include "code-block.h"
#include "node-visitor.h"
USE_SWIFT_NS


DeinitializerDef::DeinitializerDef()
    :Declaration(NodeType::Deinit), body(NULL)
{
}
DeinitializerDef::~DeinitializerDef()
{

}

void DeinitializerDef::serialize(std::wostream& out)
{
    out<<L"deinit ";
    getBody()->serialize(out);
}

void DeinitializerDef::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitDeinit);
}


CodeBlockPtr DeinitializerDef::getBody()
{
    return body;
}
void DeinitializerDef::setBody(const CodeBlockPtr& body)
{
    this->body = body;
}