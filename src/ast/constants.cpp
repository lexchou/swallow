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
    SafeDeleteAll(constants);
}


void Constants::add(Constant* constant)
{
    constants.push_back(constant);
}
int Constants::numConstants()
{
    return constants.size();
}
Constant* Constants::getConstant(int i)
{
    return constants[i];
}


void Constants::serialize(std::wostream& out)
{
}

void Constants::accept(NodeVisitor* visitor)
{
    visitor->visitConstants(this);
}