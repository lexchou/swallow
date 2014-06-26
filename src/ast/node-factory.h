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
class ParenthesizedExpression;
class Initializer;
class SelfExpression;
class DynamicType;
class ForcedValue;
class OptionalChaining;
class FunctionCall;
class ForLoop;
class WhileLoop;
class DoLoop;
class IfStatement;
class SwitchCase;
class CaseStatement;
class ReturnStatement;
class BreakStatement;
class ContinueStatement;
class FallthroughStatement;
class LabeledStatement;
class CodeBlock;
class Tuple;
class LetBinding;
class VarBinding;
class FunctionType;
class ArrayType;
class OptionalType;
class ImplicitlyUnwrappedOptional;

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
    virtual MemberAccess* createMemberAccess(ExpressionNode* self, Identifier* field);
    virtual Subscript* createSubscript(ExpressionNode* self, ExpressionNode* index);
    
    virtual TypeCheck* createTypeCheck(ExpressionNode* expr, TypeNode* type);
    virtual TypeCast* createTypeCast(ExpressionNode* expr, TypeNode* type);
    virtual Assignment* createAssignment(ExpressionNode* lhs, ExpressionNode* rhs);
    virtual ConditionalOperator* createConditionalOperator(ExpressionNode* cond, ExpressionNode* trueExpr, ExpressionNode* falseExpr);
    virtual ParenthesizedExpression* createParenthesizedExpression();
    
    virtual Initializer* createInitializer(ExpressionNode*);
    virtual SelfExpression* createSelfExpression(ExpressionNode* expr);
    virtual DynamicType* createDynamicType(ExpressionNode* expr);
    virtual ForcedValue* createForcedValue(ExpressionNode* expr);
    virtual OptionalChaining* createOptionalChaining(ExpressionNode* expr);
    virtual FunctionCall* createFunctionCall();

    virtual ForLoop* createForLoop();
    virtual WhileLoop* createWhileLoop();
    virtual IfStatement* createIf();
    virtual DoLoop* createDoLoop();
    virtual SwitchCase* createSwitch();
    virtual CaseStatement* createCase();
    virtual BreakStatement* createBreak();
    virtual ContinueStatement* createContinue();
    virtual FallthroughStatement* createFallthrough();
    virtual ReturnStatement* createReturn();
    virtual LabeledStatement* createLabel();
    virtual CodeBlock* createCodeBlock();
    virtual LetBinding* createLetBinding();
    virtual VarBinding* createVarBinding();
    virtual Tuple* createTuple();
    
    virtual FunctionType* createFunctionType(TypeNode* argumentsType, TypeNode* retType);
    virtual ArrayType* createArrayType(TypeNode* innerType);
    virtual OptionalType* createOptionalType(TypeNode* innerType);
    virtual ImplicitlyUnwrappedOptional* createImplicitlyUnwrappedOptional(TypeNode* innerType);
    
};


SWIFT_NS_END


#endif//NODE_FACTORY_H
