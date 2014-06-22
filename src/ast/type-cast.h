#ifndef TYPE_CAST_H
#define TYPE_CAST_H
#include "binary-operator.h"
#include <string>

SWIFT_NS_BEGIN
class TypeNode;
class TypeCast : public BinaryOperator
{
public:
    TypeCast(ExpressionNode* expr, TypeNode* type);
public:
    TypeNode* getType();
    void setType(TypeNode* type);
};

SWIFT_NS_END



#endif//TYPE_CAST_H
