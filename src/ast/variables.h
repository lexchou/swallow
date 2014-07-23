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

    std::vector<Variable*>::iterator begin() {return variables.begin();}
    std::vector<Variable*>::iterator end() {return variables.end();}
public://Node
    virtual void serialize(std::wostream& out);
    virtual void accept(NodeVisitor* visitor);
public:
    std::vector<Variable*> variables;
};

SWIFT_NS_END

#endif//VARIABLES_H
