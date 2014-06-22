#ifndef NODE_FACTORY_H
#define NODE_FACTORY_H
#include "swift_types.h"
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
class TypeNode;
class TypeCheck;
class TypeCast;
class Assignment;
class ConditionalOperator;

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
    virtual UnaryOperator* createUnary(const std::wstring& op, OperatorType::T type);
    virtual BinaryOperator* createBinary(const std::wstring& op, Associativity::T associativity, int precedence);

    virtual Identifier* createIdentifier(const std::wstring& op);
    virtual InOutParameter* createInOutParameter(Identifier* identifier);

    virtual ArrayLiteral* createArrayLiteral();
    virtual DictionaryLiteral* createDictionaryLiteral();
    virtual CompileConstant* createCompilecConstant(const std::wstring& name, const std::wstring& value);
    virtual MemberAccess* createMemberAccess(Identifier* self, Identifier* field);
    virtual Subscript* createSubscript(Identifier* self, ExpressionNode* index);
    
    virtual TypeCheck* createTypeCheck(ExpressionNode* expr, TypeNode* type);
    virtual TypeCast* createTypeCast(ExpressionNode* expr, TypeNode* type);
    virtual Assignment* createAssignment(ExpressionNode* lhs, ExpressionNode* rhs);
    virtual ConditionalOperator* createConditionalOperator(ExpressionNode* cond, ExpressionNode* trueExpr, ExpressionNode* falseExpr);
};


SWIFT_NS_END


#endif//NODE_FACTORY_H
