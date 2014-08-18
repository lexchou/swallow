#ifndef PARAMETERS_H
#define PARAMETERS_H
#include "Node.h"
#include <string>
#include "ast-decl.h"

SWIFT_NS_BEGIN

class Parameter;
class Parameters : public Node
{
public:
    Parameters();
    ~Parameters();
public:
    virtual void accept(NodeVisitor* visitor);
public:
    void setVariadicParameters(bool val);
    bool isVariadicParameters()const;
    
    void addParameter(const ParameterPtr& parameter);
    int numParameters()const;
    ParameterPtr getParameter(int i);

    std::vector<ParameterPtr>::iterator begin() { return parameters.begin();}
    std::vector<ParameterPtr>::iterator end() { return parameters.end();}

private:
    bool variadicParameters;
    std::vector<ParameterPtr> parameters;
};

SWIFT_NS_END

#endif//PARAMETERS_H
