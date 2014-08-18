#include "DeinitializerDef.h"
#include "NodeVisitor.h"
USE_SWIFT_NS


DeinitializerDef::DeinitializerDef()
    :Declaration(NodeType::Deinit), body(NULL)
{
}
DeinitializerDef::~DeinitializerDef()
{

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
