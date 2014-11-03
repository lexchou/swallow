#ifndef VALUE_BINDING_H
#define VALUE_BINDING_H
#include "Declaration.h"
#include <string>

SWIFT_NS_BEGIN

class Pattern;
class Expression;
typedef std::shared_ptr<class Type> TypePtr;
class ValueBinding : public Declaration
{
    friend class ValueBindings;
public:
    ValueBinding();
public:
    using Declaration::setSpecifiers;
    using Declaration::getSpecifiers;
public:

    void setTypeAttributes(const Attributes& attrs);
    const Attributes& getTypeAttributes() const;

    void setDeclaredType(const TypeNodePtr& t);
    TypeNodePtr getDeclaredType();

    const TypePtr& getType()const;
    void setType(const TypePtr& type);

    const PatternPtr& getName()const;
    void setName(const PatternPtr&);

    void setInitializer(const ExpressionPtr& initializer);
    const ExpressionPtr& getInitializer()const;

    std::shared_ptr<class ValueBindings> getOwner() const;
public:
    virtual void accept(NodeVisitor* visitor) override;
private:
    PatternPtr name;
    ExpressionPtr initializer;
    TypePtr type;
    TypeNodePtr declaredType;
    Attributes typeAttributes;
    std::weak_ptr<class ValueBindings> owner;
};

SWIFT_NS_END

#endif//VALUE_BINDING_H
