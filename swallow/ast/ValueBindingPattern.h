#ifndef VALUE_BINDING_PATTERN_H
#define VALUE_BINDING_PATTERN_H
#include "Pattern.h"
#include <string>

SWIFT_NS_BEGIN

class ValueBindingPattern : public Pattern
{
public:
    ValueBindingPattern();
    ~ValueBindingPattern();
public:
    void setBinding(const PatternPtr& st);
    PatternPtr getBinding() const;

    bool isReadOnly() const;
    void setReadOnly(bool readonly);

    TypeNodePtr getDeclaredType()const;
    void setDeclaredType(const TypeNodePtr& type);
public:
    void accept(NodeVisitor* visitor);
protected:
    TypeNodePtr declaredType;
    bool readonly;
    PatternPtr binding;
};



SWIFT_NS_END

#endif//VALUE_BINDING_PATTERN_H
