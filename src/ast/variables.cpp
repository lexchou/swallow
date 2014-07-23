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
    SafeDeleteAll(variables);
}
void Variables::serialize(std::wostream& out)
{
    out<<L"let ";
    std::vector<Variable*>::iterator iter = variables.begin();
    for(; iter != variables.end(); iter++)
    {
        if(iter != variables.begin())
            out<<L", ";
        (*iter)->serialize(out);
    }
}
void Variables::accept(NodeVisitor* visitor)
{
    visitor->visitVariables(this);
}


void Variables::addVariable(Variable* var)
{
    variables.push_back(var);
}
Variable* Variables::getVariable(int i)
{
    return variables[i];
}
int Variables::numVariables()
{
    return variables.size();
}

