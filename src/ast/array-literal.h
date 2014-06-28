#ifndef ARRAY_LITERAL_H
#define ARRAY_LITERAL_H
#include "expression.h"

SWIFT_NS_BEGIN

class ArrayLiteral : public Expression
{
public:
	ArrayLiteral();
public:
	void push(Expression* item);
public:
    virtual void serialize(std::wostream& out);
};
SWIFT_NS_END

#endif//ARRAY_LITERAL_H