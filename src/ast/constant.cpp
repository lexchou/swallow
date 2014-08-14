#include <ast/node-visitor.h>
#include "constant.h"
#include "pattern.h"
#include "expression.h"
USE_SWIFT_NS


Constant::Constant()
    :ValueBinding(NodeType::Constant)
{
}
Constant::~Constant()
{

}
void Constant::serialize(std::wostream& out)
{
}

void Constant::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitConstant);
}