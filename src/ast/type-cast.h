#ifndef TYPE_CAST_H
#define TYPE_CAST_H
#include "binary-operator.h"
#include <string>

SWIFT_NS_BEGIN
class TypeNode;
class TypeCast : public BinaryOperator
{
public:
    TypeCast(Expression* expr, TypeNode* type);
    ~TypeCast();
public:
    TypeNode* getType();
    void setType(TypeNode* type);
public:
    virtual int numChildren();
    virtual Node* get(int i);
    virtual void set(int i, Node* val);
private:
    TypeNode* type;
};

SWIFT_NS_END



#endif//TYPE_CAST_H
