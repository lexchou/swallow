#include <ast/node-visitor.h>
#include "variables.h"
#include "variable.h"
USE_SWIFT_NS


Variables::Variables()
    :Declaration(NodeType::Variables)
{
}
Variables::~Variables()
{
}
void Variables::serialize(std::wostream& out)
{
    out<<L"let ";
    std::vector<VariablePtr>::iterator iter = variables.begin();
    for(; iter != variables.end(); iter++)
    {
        if(iter != variables.begin())
            out<<L", ";
        (*iter)->serialize(out);
    }
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

