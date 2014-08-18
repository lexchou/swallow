#include "Constant.h"
#include "NodeVisitor.h"
USE_SWIFT_NS


Constant::Constant()
    :ValueBinding(NodeType::Constant)
{
}
Constant::~Constant()
{

}


void Constant::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitConstant);
}
