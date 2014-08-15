#include "constants.h"
#include <ast/node-visitor.h>
USE_SWIFT_NS


Constants::Constants()
:Declaration(NodeType::Constants)
{
}
Constants::~Constants()
{

}


void Constants::add(const ConstantPtr& constant)
{
    constants.push_back(constant);
}
int Constants::numConstants()
{
    return constants.size();
}
ConstantPtr Constants::getConstant(int i)
{
    return constants[i];
}


void Constants::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitConstants);
}