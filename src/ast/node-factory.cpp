#include "node-factory.h"
#include "ast/ast.h"
USE_SWIFT_NS;

CommentNode* NodeFactory::createComment(const std::wstring& comment)
{
}
BooleanLiteral* NodeFactory::createBoolean(bool value)
{
    return new BooleanLiteral(value);
}
IntegerLiteral* NodeFactory::createInteger(const std::wstring& value)
{
    return new IntegerLiteral(value);
}
FloatLiteral* NodeFactory::createFloat(const std::wstring& value)
{
    return new FloatLiteral(value);
}
StringLiteral* NodeFactory::createString(const std::wstring& value)
{
    return new StringLiteral(value);
}
UnaryOperator* NodeFactory::createUnary(const std::wstring& op, Node* operand)
{
    UnaryOperator* ret = new UnaryOperator(op);
    ret->setOperand(operand);
    return ret;
}
BinaryOperator* NodeFactory::createBinary(const std::wstring& op, Node* lhs, Node* rhs)
{
    BinaryOperator* ret = new BinaryOperator(op);
    ret->setLHS(lhs);
    ret->setRHS(rhs);
    return ret;
}
