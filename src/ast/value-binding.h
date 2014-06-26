#ifndef VALUE_BINDING_H
#define VALUE_BINDING_H
#include "statement.h"
#include <string>

SWIFT_NS_BEGIN

class ValueBinding : public Statement
{
public:
    ValueBinding();
public:
    void setBinding(Statement* st);
    Statement* getBinding();
private:
};

class LetBinding : public ValueBinding
{
public:
    virtual void serialize(std::wostream& out);
};

class VarBinding : public ValueBinding
{
public:
    virtual void serialize(std::wostream& out);
};

SWIFT_NS_END

#endif//VALUE_BINDING_H
