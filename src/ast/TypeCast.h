#ifndef TYPE_CAST_H
#define TYPE_CAST_H
#include "BinaryOperator.h"
#include <string>

SWIFT_NS_BEGIN
class TypeNode;
class TypeCast : public BinaryOperator
{
public:
    TypeCast();
    ~TypeCast();
public:
    TypeNodePtr getDeclaredType();
    void setDeclaredType(const TypeNodePtr& type);
public:
    virtual int numChildren();
    virtual NodePtr get(int i);
    virtual void set(int i, const NodePtr& val);
    
    void setOptional(bool val);
    bool isOptional()const;
private:
    TypeNodePtr declaredType;
    bool optional;
};

SWIFT_NS_END



#endif//TYPE_CAST_H
