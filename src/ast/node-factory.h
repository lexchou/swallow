#ifndef NODE_FACTORY_H
#define NODE_FACTORY_H
#include "swift_types.h"
#include <string>
#include <vector>

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
class Expression;
class InOutParameter;
class ArrayLiteral;
class DictionaryLiteral;
class CompileConstant;
class MemberAccess;
class SubscriptAccess;
class TypeNode;
class TypeCheck;
class TypeCast;
class Assignment;
class ConditionalOperator;
class ParenthesizedExpression;
class InitializerReference;
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
class TypeIdentifier;
class ProtocolComposition;
class TupleType;
class Attribute;



class Import;
class Constant;
class Variables;
class Variable;
class TypeAlias;
class FunctionDef;
class EnumDef;
class StructDef;
class ClassDef;
class ProtocolDef;
class InitializerDef;
class DeinitializerDef;
class ExtensionDef;
class SubscriptDef;
class OperatorDef;

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
    virtual MemberAccess* createMemberAccess(Expression* self, Identifier* field);
    virtual SubscriptAccess* createSubscriptAccess(Expression* self, Expression* index);
    
    virtual TypeCheck* createTypeCheck(Expression* expr, TypeNode* type);
    virtual TypeCast* createTypeCast(Expression* expr, TypeNode* type);
    virtual Assignment* createAssignment(Expression* lhs, Expression* rhs);
    virtual ConditionalOperator* createConditionalOperator(Expression* cond, Expression* trueExpr, Expression* falseExpr);
    virtual ParenthesizedExpression* createParenthesizedExpression();
    
    virtual InitializerReference* createInitializerReference(Expression*);
    virtual SelfExpression* createSelfExpression(Expression* expr);
    virtual DynamicType* createDynamicType(Expression* expr);
    virtual ForcedValue* createForcedValue(Expression* expr);
    virtual OptionalChaining* createOptionalChaining(Expression* expr);
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
    virtual TypeIdentifier* createTypeIdentifier(const std::wstring& typeName);
    virtual ProtocolComposition* createProtocolComposition();
    virtual TupleType* createTupleType();
    virtual Attribute* createAttribute(const std::wstring& name);

    
    
    virtual Import* createImport(const std::vector<Attribute*>& attrs);
    virtual Constant* createConstant(const std::vector<Attribute*>& attrs, int specifiers);
    virtual Variables* createVariables(const std::vector<Attribute*>& attrs, int specifiers);
    virtual Variable* createVariable();
    virtual TypeAlias* createTypealias(const std::vector<Attribute*>& attrs);
    virtual FunctionDef* createFunction(const std::wstring& name, const std::vector<Attribute*>& attrs);
    virtual EnumDef* createEnum(const std::wstring& name, const std::vector<Attribute*>& attrs);
    virtual StructDef* createStruct(const std::wstring& name, const std::vector<Attribute*>& attrs);
    virtual ClassDef* createClass(const std::wstring& name, const std::vector<Attribute*>& attrs);
    virtual ProtocolDef* createProtocol(const std::wstring& name, const std::vector<Attribute*>& attrs);
    virtual InitializerDef* createInitializer(const std::vector<Attribute*>& attrs);
    virtual DeinitializerDef* createDeinitializer(const std::vector<Attribute*>& attrs);
    virtual ExtensionDef* createExtension(const std::vector<Attribute*>& attrs);
    virtual SubscriptDef* createSubscript(const std::vector<Attribute*>& attrs, int specifiers);
    virtual OperatorDef* createOperator(const std::vector<Attribute*>& attrs);
    
    
};


SWIFT_NS_END


#endif//NODE_FACTORY_H
