#ifndef VALUE_BINDING_H
#define VALUE_BINDING_H
#include "pattern.h"
#include <string>

SWIFT_NS_BEGIN

class ValueBinding : public Pattern
{
public:
    ValueBinding(NodeType::T nodeType);
    ~ValueBinding();
public:
    void setBinding(Pattern* st);
    Pattern* getBinding();
protected:
    Pattern* binding;
};

class LetBinding : public ValueBinding
{
public:
    LetBinding();
    virtual void serialize(std::wostream& out);
};

class VarBinding : public ValueBinding
{
public:
    VarBinding();
    virtual void serialize(std::wostream& out);
};

SWIFT_NS_END

#endif//VALUE_BINDING_H
