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
class Identifier;
class Node;
class ExpressionNode;
class InOutParameter;
class ArrayLiteral;
class DictionaryLiteral;
class CompileConstant;
class MemberAccess;
class Subscript;

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
    virtual UnaryOperator* createUnary(const std::wstring& op, ExpressionNode* operand);
    virtual BinaryOperator* createBinary(const std::wstring& op, ExpressionNode* lhs, ExpressionNode* rhs);

    virtual Identifier* createIdentifier(const std::wstring& op);
    virtual InOutParameter* createInOutParameter(Identifier* identifier);

    virtual ArrayLiteral* createArrayLiteral();
    virtual DictionaryLiteral* createDictionaryLiteral();
    virtual CompileConstant* createCompilecConstant(const std::wstring& name, const std::wstring& value);
    virtual MemberAccess* createMemberAccess(Identifier* self, Identifier* field);
    virtual Subscript* createSubscript(Identifier* self, ExpressionNode* index);
};


SWIFT_NS_END


#endif//NODE_FACTORY_H
