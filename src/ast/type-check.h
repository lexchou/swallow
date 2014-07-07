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
    ~TypeCheck();
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



#endif//TYPE_CHECK_H
