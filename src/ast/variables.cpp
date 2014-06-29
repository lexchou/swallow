
#include "variables.h"
#include "variable.h"
USE_SWIFT_NS


Variables::Variables()
    :Declaration(0)
{
}
void Variables::serialize(std::wostream& out)
{
    out<<L"let ";
    Children::iterator iter = children.begin();
    for(; iter != children.end(); iter++)
    {
        if(iter != children.begin())
            out<<L", ";
        (*iter)->serialize(out);
    }
}


void Variables::addVariable(Variable* var)
{
    children.push_back(var);
}
Variable* Variables::getVariable(int i)
{
    return static_cast<Variable*>(get(i));
}
int Variables::numVariables()
{
    return children.size();
}