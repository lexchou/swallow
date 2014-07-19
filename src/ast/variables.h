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
    void addVariable(Variable* var);
    Variable* getVariable(int i);
    int numVariables();
public://Node
    virtual void accept(NodeVisitor* visitor);

public:
    virtual void serialize(std::wostream& out);
public:
    std::vector<Variable*> variables;
};

SWIFT_NS_END

#endif//VARIABLES_H
