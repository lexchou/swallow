#ifndef TYPE_CHECK_H
#define TYPE_CHECK_H
#include "binary-operator.h"
#include <string>

SWIFT_NS_BEGIN
class TypeNode;
class TypeCheck : public BinaryOperator
{
public:
    TypeCheck();
    ~TypeCheck();
public:
    TypeNodePtr getDeclaredType();
    void setDeclaredType(const TypeNodePtr& type);
public:
    virtual int numChildren();
    virtual NodePtr get(int i);
    virtual void set(int i, const NodePtr& val);
private:
    TypeNodePtr declaredType;
};

SWIFT_NS_END



#endif//TYPE_CHECK_H
