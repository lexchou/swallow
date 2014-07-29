
#include "class-def.h"
#include "node-visitor.h"
USE_SWIFT_NS


ClassDef::ClassDef()
    :TypeDeclaration(NodeType::Class)
{
}
void ClassDef::serialize(std::wostream& out)
{
}
void ClassDef::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitClass);

}