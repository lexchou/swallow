#ifndef NODE_FACTORY_H
#define NODE_FACTORY_H
#include "swift_conf.h"
#include <string>

SWIFT_NS_BEGIN

class BooleanLiteral;
class IntegerLiteral;
class FloatLiteral;
class StringLiteral;
class CommentNode;
class UnaryOperator;
class BinaryOperator;
class Node;

class NodeFactory
{
public:
    virtual ~NodeFactory(){}
public:
    virtual CommentNode* createComment(const std::wstring& comment);
    virtual BooleanLiteral* createBoolean(bool value);
    virtual IntegerLiteral* createInteger(const std::wstring& value);
    virtual FloatLiteral* createFloat(const std::wstring& value);
    virtual StringLiteral* createString(const std::wstring& value);
    virtual UnaryOperator* createUnary(const std::wstring& op, Node* operand);
    virtual BinaryOperator* createBinary(const std::wstring& op, Node* lhs, Node* rhs);


};


SWIFT_NS_END


#endif//NODE_FACTORY_H
