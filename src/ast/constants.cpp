#include <ast/node-visitor.h>
#include <ast/constant.h>
#include "constants.h"
#include "pattern.h"
#include "expression.h"
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


void Constants::serialize(std::wostream& out)
{
}

void Constants::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitConstants);
}