#ifndef ARRAY_LITERAL_H
#define ARRAY_LITERAL_H
#include "expression-node.h"

SWIFT_NS_BEGIN

class ArrayLiteral : public ExpressionNode
{
public:
	ArrayLiteral();
public:
	void push(ExpressionNode* item);
};
SWIFT_NS_END

#endif//ARRAY_LITERAL_H