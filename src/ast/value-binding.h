#ifndef VALUE_BINDING_H
#define VALUE_BINDING_H
#include "pattern.h"
#include <string>

SWIFT_NS_BEGIN

class ValueBinding : public Pattern
{
public:
    ValueBinding();
public:
    void setBinding(Pattern* st);
    Pattern* getBinding();
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
