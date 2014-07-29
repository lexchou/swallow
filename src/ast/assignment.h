#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H
#include "binary-operator.h"

SWIFT_NS_BEGIN

class Assignment : public BinaryOperator
{
public:
	Assignment();
public:
    virtual void accept(NodeVisitor* visitor);
};
SWIFT_NS_END

#endif//ASSIGNMENT_H