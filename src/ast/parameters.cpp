
#include "parameters.h"
#include "parameter.h"
#include "node-visitor.h"
USE_SWIFT_NS


Parameters::Parameters()
:Node(NodeType::Parameters)
{
    variadicParameters = false;
}
Parameters::~Parameters()
{

}

void Parameters::serialize(std::wostream& out)
{
}
void Parameters::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitParameters);
}

void Parameters::setVariadicParameters(bool val)
{
    variadicParameters = val;
}
bool Parameters::isVariadicParameters()const
{
    return variadicParameters;
}

void Parameters::addParameter(const ParameterPtr& parameter)
{
    parameters.push_back(parameter);
}
int Parameters::numParameters()const
{
    return parameters.size();
}
ParameterPtr Parameters::getParameter(int i)
{
    return parameters[i];
}
