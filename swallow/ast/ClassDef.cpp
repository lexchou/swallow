#include "ClassDef.h"
#include "NodeVisitor.h"
USE_SWIFT_NS


ClassDef::ClassDef()
    :TypeDeclaration(NodeType::Class)
{
}

void ClassDef::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitClass);

}
