#include "array-literal.h"
USE_SWIFT_NS

ArrayLiteral::ArrayLiteral()
	:ExpressionNode(0)
{

}
void ArrayLiteral::push(ExpressionNode* item)
{
	children.push_back(item);
}

