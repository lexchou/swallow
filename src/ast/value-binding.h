#ifndef VALUE_BINDING_H
#define VALUE_BINDING_H
#include "declaration.h"
#include <string>

SWIFT_NS_BEGIN

class Pattern;
class Expression;
typedef std::shared_ptr<class Type> TypePtr;
class ValueBinding : public Declaration
{
public:
    ValueBinding(NodeType::T nodeType)
            :Declaration(nodeType)
    {}
public:
    using Declaration::setSpecifiers;
    using Declaration::getSpecifiers;
public:
    const TypePtr& getType()const;
    void setType(const TypePtr& type);

    const PatternPtr& getName()const;
    void setName(const PatternPtr&);

    void setInitializer(const ExpressionPtr& initializer);
    const ExpressionPtr& getInitializer()const;
public:
    PatternPtr name;
    ExpressionPtr initializer;
    TypePtr type;
};

SWIFT_NS_END

#endif//VALUE_BINDING_H
