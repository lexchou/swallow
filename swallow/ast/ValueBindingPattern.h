#ifndef VALUE_BINDING_PATTERN_H
#define VALUE_BINDING_PATTERN_H
#include "Pattern.h"
#include <string>

SWIFT_NS_BEGIN

class ValueBindingPattern : public Pattern
{
public:
    ValueBindingPattern(NodeType::T nodeType);
    ~ValueBindingPattern();
public:
    void setBinding(const PatternPtr& st);
    PatternPtr getBinding();
protected:
    PatternPtr binding;
};

class LetPattern : public ValueBindingPattern
{
public:
    LetPattern();
    virtual void accept(NodeVisitor* visitor);
};

class VarPattern : public ValueBindingPattern
{
public:
    VarPattern();
    virtual void accept(NodeVisitor* visitor);
};

SWIFT_NS_END

#endif//VALUE_BINDING_PATTERN_H
