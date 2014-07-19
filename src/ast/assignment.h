#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H
#include "binary-operator.h"

SWIFT_NS_BEGIN

class Assignment : public BinaryOperator
{
public:
	Assignment(Expression* lhs, Expression* rhs);
public:
    virtual void accept(NodeVisitor* visitor);
};
SWIFT_NS_END

#endif//ASSIGNMENT_H