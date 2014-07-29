#ifndef VARIABLES_H
#define VARIABLES_H
#include "declaration.h"
#include <string>

SWIFT_NS_BEGIN

class Variable;

class Variables : public Declaration
{
public:
    Variables();
    ~Variables();
public:
    using Declaration::setAttributes;
    using Declaration::setSpecifiers;
    using Declaration::getAttributes;
    using Declaration::getSpecifiers;
public:
    void addVariable(const VariablePtr& var);
    VariablePtr getVariable(int i);
    int numVariables();

    std::vector<VariablePtr>::iterator begin() {return variables.begin();}
    std::vector<VariablePtr>::iterator end() {return variables.end();}
public://Node
    virtual void serialize(std::wostream& out);
    virtual void accept(NodeVisitor* visitor);
public:
    std::vector<VariablePtr> variables;
};

SWIFT_NS_END

#endif//VARIABLES_H
