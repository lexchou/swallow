#ifndef TYPED_PATTERN_H
#define TYPED_PATTERN_H
#include "Expression.h"
#include <string>

SWIFT_NS_BEGIN
class TypeNode;
class GenericArgument;
class Type;
class TypedPattern : public Expression
{
public:
    TypedPattern();
    ~TypedPattern();
public:
    virtual void accept(NodeVisitor* visitor);
public:
    void setPattern(const PatternPtr& pattern);
    PatternPtr getPattern()const;
    
    void setDeclaredType(const TypeNodePtr& type);
    TypeNodePtr getDeclaredType();
    
    GenericArgumentDefPtr getGenericArgumentDef();
    void setGenericArgumentDef(const GenericArgumentDefPtr& val);
protected:
    PatternPtr pattern;
    TypeNodePtr declaredType;
    GenericArgumentDefPtr genericArgumentDef;
};

SWIFT_NS_END



#endif//TYPED_PATTERN_H
