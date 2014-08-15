#ifndef ARRAY_LITERAL_H
#define ARRAY_LITERAL_H
#include "expression.h"

SWIFT_NS_BEGIN

class ArrayLiteral : public Expression
{
public:
	ArrayLiteral();
    ~ArrayLiteral();
public:
	void push(const ExpressionPtr& item);
    ExpressionPtr getElement(int i);
    int numElements()const;
public:
    virtual void accept(NodeVisitor* visitor);


public:
    std::vector<ExpressionPtr> elements;
};

SWIFT_NS_END

#endif//ARRAY_LITERAL_H