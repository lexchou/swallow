#ifndef TYPE_CHECK_H
#define TYPE_CHECK_H
#include "binary-operator.h"
#include <string>

SWIFT_NS_BEGIN
class TypeNode;
class TypeCheck : public BinaryOperator
{
public:
    TypeCheck(Pattern* expr, TypeNode* type);
public:
    TypeNode* getType();
    void setType(TypeNode* type);
};

SWIFT_NS_END



#endif//TYPE_CHECK_H
