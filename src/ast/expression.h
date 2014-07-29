#ifndef EXPRESSION_H
#define EXPRESSION_H
#include "pattern.h"
SWIFT_NS_BEGIN


class Expression : public Pattern
{
protected:
	Expression(NodeType::T nodeType);
};
typedef std::shared_ptr<Expression> ExpressionPtr;
SWIFT_NS_END
#endif//EXPRESSION_H