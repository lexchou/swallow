#ifndef PARAMETERS_H
#define PARAMETERS_H
#include "node.h"
#include <string>

SWIFT_NS_BEGIN

class Parameter;
class Parameters : public Node
{
public:
    Parameters();
public:
    virtual void serialize(std::wostream& out);
public:
    void setVariadicParameters(bool val);
    bool isVariadicParameters()const;
    
    void addParameter(Parameter* parameter);
    int numParameters()const;
    Parameter* getParameter(int i);
private:
    bool variadicParameters;
};

SWIFT_NS_END

#endif//PARAMETERS_H
