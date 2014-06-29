
#include "parameters.h"
#include "parameter.h"
USE_SWIFT_NS


Parameters::Parameters()
    :Node(0)
{
    variadicParameters = false;
}
void Parameters::serialize(std::wostream& out)
{
}


void Parameters::setVariadicParameters(bool val)
{
    variadicParameters = val;
}
bool Parameters::isVariadicParameters()const
{
    return variadicParameters;
}

void Parameters::addParameter(Parameter* parameter)
{
    children.push_back(parameter);
}
int Parameters::numParameters()const
{
    return children.size();
}
Parameter* Parameters::getParameter(int i)
{
    return static_cast<Parameter*>(get(i));
}
