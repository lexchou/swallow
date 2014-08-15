#include <ast/node-visitor.h>
#include "variables.h"
USE_SWIFT_NS


Variables::Variables()
    :Declaration(NodeType::Variables)
{
}
Variables::~Variables()
{
}

void Variables::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitVariables);
}


void Variables::addVariable(const VariablePtr& var)
{
    variables.push_back(var);
}
VariablePtr Variables::getVariable(int i)
{
    return variables[i];
}
int Variables::numVariables()
{
    return variables.size();
}

