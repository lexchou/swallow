#ifndef EXPRESSION_NODE_H
#define EXPRESSION_NODE_H
#include "statement.h"
SWIFT_NS_BEGIN


class ExpressionNode : public Statement
{
protected:
	ExpressionNode(int numChildren);
};

SWIFT_NS_END
#endif//EXPRESSION_NODE_H