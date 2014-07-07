
#include "parameters.h"
#include "parameter.h"
USE_SWIFT_NS


Parameters::Parameters()
{
    variadicParameters = false;
}
Parameters::~Parameters()
{
    SafeDeleteAll(parameters);
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
    parameters.push_back(parameter);
}
int Parameters::numParameters()const
{
    return parameters.size();
}
Parameter* Parameters::getParameter(int i)
{
    return static_cast<Parameter*>(parameters[i]);
}
