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
class GenericArgument;
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
class ForInLoop;
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
class Closure;



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
class Parameters;
class Parameter;
class GenericConstraint;
class GenericParameters;
class Pattern;
class EnumCasePattern;
struct SourceInfo;
class AutoReleasePool;

class NodeFactory
{
public:
    NodeFactory();
    virtual ~NodeFactory(){}
public:
    virtual CommentNode* createComment(const SourceInfo& state, const std::wstring& comment);
    virtual BooleanLiteral* createBoolean(const SourceInfo& state, bool value);
    virtual IntegerLiteral* createInteger(const SourceInfo& state, const std::wstring& value);
    virtual FloatLiteral* createFloat(const SourceInfo& state, const std::wstring& value);
    virtual StringLiteral* createString(const SourceInfo& state, const std::wstring& value);
    virtual UnaryOperator* createUnary(const SourceInfo& state, const std::wstring& op, OperatorType::T type);
    virtual BinaryOperator* createBinary(const SourceInfo& state, const std::wstring& op, Associativity::T associativity, int precedence);

    virtual Identifier* createIdentifier(const SourceInfo& state, const std::wstring& op);
    virtual GenericArgument* createGenericArgument(const SourceInfo& state);
    virtual InOutParameter* createInOutParameter(const SourceInfo& state, Identifier* identifier);

    virtual ArrayLiteral* createArrayLiteral(const SourceInfo& state);
    virtual DictionaryLiteral* createDictionaryLiteral(const SourceInfo& state);
    virtual CompileConstant* createCompilecConstant(const SourceInfo& state, const std::wstring& name, const std::wstring& value);
    virtual MemberAccess* createMemberAccess(const SourceInfo& state, Expression* self, Identifier* field);
    virtual SubscriptAccess* createSubscriptAccess(const SourceInfo& state, Expression* self, Expression* index);
    
    virtual TypeCheck* createTypeCheck(const SourceInfo& state, Pattern* expr, TypeNode* type);
    virtual TypeCast* createTypeCast(const SourceInfo& state, Expression* expr, TypeNode* type);
    virtual Assignment* createAssignment(const SourceInfo& state, Expression* lhs, Expression* rhs);
    virtual ConditionalOperator* createConditionalOperator(const SourceInfo& state, Expression* cond, Expression* trueExpr, Expression* falseExpr);
    virtual ParenthesizedExpression* createParenthesizedExpression(const SourceInfo& state);
    
    virtual InitializerReference* createInitializerReference(const SourceInfo& state, Expression*);
    virtual SelfExpression* createSelfExpression(const SourceInfo& state, Expression* expr);
    virtual DynamicType* createDynamicType(const SourceInfo& state, Expression* expr);
    virtual ForcedValue* createForcedValue(const SourceInfo& state, Expression* expr);
    virtual OptionalChaining* createOptionalChaining(const SourceInfo& state, Expression* expr);
    virtual FunctionCall* createFunctionCall(const SourceInfo& state);

    virtual ForLoop* createForLoop(const SourceInfo& state);
    virtual ForInLoop* createForInLoop(const SourceInfo& state);
    virtual WhileLoop* createWhileLoop(const SourceInfo& state);
    virtual IfStatement* createIf(const SourceInfo& state);
    virtual DoLoop* createDoLoop(const SourceInfo& state);
    virtual SwitchCase* createSwitch(const SourceInfo& state);
    virtual CaseStatement* createCase(const SourceInfo& state);
    virtual BreakStatement* createBreak(const SourceInfo& state);
    virtual ContinueStatement* createContinue(const SourceInfo& state);
    virtual FallthroughStatement* createFallthrough(const SourceInfo& state);
    virtual ReturnStatement* createReturn(const SourceInfo& state);
    virtual LabeledStatement* createLabel(const SourceInfo& state);
    virtual CodeBlock* createCodeBlock(const SourceInfo& state);
    virtual LetBinding* createLetBinding(const SourceInfo& state);
    virtual VarBinding* createVarBinding(const SourceInfo& state);
    virtual Tuple* createTuple(const SourceInfo& state);
    virtual Closure* createClosure(const SourceInfo& state);
    virtual EnumCasePattern* createEnumCasePattern(const SourceInfo& state, const std::wstring& name);
    
    virtual FunctionType* createFunctionType(const SourceInfo& state, TypeNode* argumentsType, TypeNode* retType);
    virtual ArrayType* createArrayType(const SourceInfo& state, TypeNode* innerType);
    virtual OptionalType* createOptionalType(const SourceInfo& state, TypeNode* innerType);
    virtual ImplicitlyUnwrappedOptional* createImplicitlyUnwrappedOptional(const SourceInfo& state, TypeNode* innerType);
    virtual TypeIdentifier* createTypeIdentifier(const SourceInfo& state, const std::wstring& typeName);
    virtual ProtocolComposition* createProtocolComposition(const SourceInfo& state);
    virtual TupleType* createTupleType(const SourceInfo& state);
    virtual Attribute* createAttribute(const SourceInfo& state);

    
    
    virtual Import* createImport(const SourceInfo& state, const std::vector<Attribute*>& attrs);
    virtual Constant* createConstant(const SourceInfo& state, const std::vector<Attribute*>& attrs, int specifiers);
    virtual Variables* createVariables(const SourceInfo& state, const std::vector<Attribute*>& attrs, int specifiers);
    virtual Variable* createVariable(const SourceInfo& state);
    virtual TypeAlias* createTypealias(const SourceInfo& state, const std::vector<Attribute*>& attrs);
    virtual FunctionDef* createFunction(const SourceInfo& state, const std::vector<Attribute*>& attrs, int specifiers);
    virtual Parameters* createParameters(const SourceInfo& state);
    virtual Parameter* createParameter(const SourceInfo& state);
    virtual EnumDef* createEnum(const SourceInfo& state, const std::vector<Attribute*>& attrs);
    virtual StructDef* createStruct(const SourceInfo& state, const std::vector<Attribute*>& attrs);
    virtual ClassDef* createClass(const SourceInfo& state, const std::vector<Attribute*>& attrs);
    virtual ProtocolDef* createProtocol(const SourceInfo& state, const std::vector<Attribute*>& attrs);
    virtual InitializerDef* createInitializer(const SourceInfo& state, const std::vector<Attribute*>& attrs);
    virtual DeinitializerDef* createDeinitializer(const SourceInfo& state, const std::vector<Attribute*>& attrs);
    virtual ExtensionDef* createExtension(const SourceInfo& state, const std::vector<Attribute*>& attrs);
    virtual SubscriptDef* createSubscript(const SourceInfo& state, const std::vector<Attribute*>& attrs);
    virtual OperatorDef* createOperator(const SourceInfo& state, const std::vector<Attribute*>& attrs);
    virtual GenericConstraint* createGenericConstraint(const SourceInfo& state);
    virtual GenericParameters* createGenericParameters(const SourceInfo& state);


public:
    void setAutoReleasePool(AutoReleasePool* autoReleasePool);
protected:

    void bindNode(const SourceInfo&s, Node* n);

    template<class T>
    inline T* _(const SourceInfo& s, T* n)
    {
        bindNode(s, static_cast<Node*>(n));
        return n;
    }

protected:
    AutoReleasePool* autoReleasePool;
};


SWIFT_NS_END


#endif//NODE_FACTORY_H
