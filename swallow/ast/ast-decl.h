#ifndef AST_DECL_H
#define AST_DECL_H
#include "swift_conf.h"
#include <memory>
#include <vector>

SWIFT_NS_BEGIN

class Node;
class Expression;
class Operator;
typedef std::shared_ptr<Operator> OperatorPtr;

class Declaration;
typedef std::shared_ptr<Declaration> DeclarationPtr;
class Statement;
typedef std::shared_ptr<Statement> StatementPtr;

class IntegerLiteral;
typedef std::shared_ptr<IntegerLiteral> IntegerLiteralPtr;
class FloatLiteral;
typedef std::shared_ptr<FloatLiteral> FloatLiteralPtr;
class StringLiteral;
typedef std::shared_ptr<StringLiteral> StringLiteralPtr;
class CommentNode;
typedef std::shared_ptr<CommentNode> CommentNodePtr;
class UnaryOperator;
typedef std::shared_ptr<UnaryOperator> UnaryOperatorPtr;
class BinaryOperator;
typedef std::shared_ptr<BinaryOperator> BinaryOperatorPtr;
class Identifier;
typedef std::shared_ptr<Identifier> IdentifierPtr;
class GenericArgument;
typedef std::shared_ptr<GenericArgument> GenericArgumentPtr;
class Node;
typedef std::shared_ptr<Node> NodePtr;
class Expression;
typedef std::shared_ptr<Expression> ExpressionPtr;
class InOutParameter;
typedef std::shared_ptr<InOutParameter> InOutParameterPtr;
class ArrayLiteral;
typedef std::shared_ptr<ArrayLiteral> ArrayLiteralPtr;
class DictionaryLiteral;
typedef std::shared_ptr<DictionaryLiteral> DictionaryLiteralPtr;
class CompileConstant;
typedef std::shared_ptr<CompileConstant> CompileConstantPtr;
class MemberAccess;
typedef std::shared_ptr<MemberAccess> MemberAccessPtr;
class SubscriptAccess;
typedef std::shared_ptr<SubscriptAccess> SubscriptAccessPtr;
class TypeNode;
typedef std::shared_ptr<TypeNode> TypeNodePtr;
class TypeCheck;
typedef std::shared_ptr<TypeCheck> TypeCheckPtr;
class TypeCast;
typedef std::shared_ptr<TypeCast> TypeCastPtr;
class Assignment;
typedef std::shared_ptr<Assignment> AssignmentPtr;
class ConditionalOperator;
typedef std::shared_ptr<ConditionalOperator> ConditionalOperatorPtr;
class ParenthesizedExpression;
typedef std::shared_ptr<ParenthesizedExpression> ParenthesizedExpressionPtr;
class InitializerReference;
typedef std::shared_ptr<InitializerReference> InitializerReferencePtr;
class SelfExpression;
typedef std::shared_ptr<SelfExpression> SelfExpressionPtr;
class DynamicType;
typedef std::shared_ptr<DynamicType> DynamicTypePtr;
class ForcedValue;
typedef std::shared_ptr<ForcedValue> ForcedValuePtr;
class OptionalChaining;
typedef std::shared_ptr<OptionalChaining> OptionalChainingPtr;
class FunctionCall;
typedef std::shared_ptr<FunctionCall> FunctionCallPtr;
class ForLoop;
typedef std::shared_ptr<ForLoop> ForLoopPtr;
class ForInLoop;
typedef std::shared_ptr<ForInLoop> ForInLoopPtr;
class WhileLoop;
typedef std::shared_ptr<WhileLoop> WhileLoopPtr;
class DoLoop;
typedef std::shared_ptr<DoLoop> DoLoopPtr;
class IfStatement;
typedef std::shared_ptr<IfStatement> IfStatementPtr;
class SwitchCase;
typedef std::shared_ptr<SwitchCase> SwitchCasePtr;
class CaseStatement;
typedef std::shared_ptr<CaseStatement> CaseStatementPtr;
class ReturnStatement;
typedef std::shared_ptr<ReturnStatement> ReturnStatementPtr;
class BreakStatement;
typedef std::shared_ptr<BreakStatement> BreakStatementPtr;
class ContinueStatement;
typedef std::shared_ptr<ContinueStatement> ContinueStatementPtr;
class FallthroughStatement;
typedef std::shared_ptr<FallthroughStatement> FallthroughStatementPtr;
class LabeledStatement;
typedef std::shared_ptr<LabeledStatement> LabeledStatementPtr;
class CodeBlock;
typedef std::shared_ptr<CodeBlock> CodeBlockPtr;
class Tuple;
typedef std::shared_ptr<Tuple> TuplePtr;
typedef std::shared_ptr<class ValueBindingPattern> ValueBindingPatternPtr;
typedef std::shared_ptr<class LetPattern> LetPatternPtr;
typedef std::shared_ptr<class VarPattern> VarPatternPtr;
class FunctionType;
typedef std::shared_ptr<FunctionType> FunctionTypePtr;
class ArrayType;
typedef std::shared_ptr<ArrayType> ArrayTypePtr;
class OptionalType;
typedef std::shared_ptr<OptionalType> OptionalTypePtr;
class ImplicitlyUnwrappedOptional;
typedef std::shared_ptr<ImplicitlyUnwrappedOptional> ImplicitlyUnwrappedOptionalPtr;
class TypeIdentifier;
typedef std::shared_ptr<TypeIdentifier> TypeIdentifierPtr;
class ProtocolComposition;
typedef std::shared_ptr<ProtocolComposition> ProtocolCompositionPtr;
class TupleType;
typedef std::shared_ptr<TupleType> TupleTypePtr;
class Attribute;
typedef std::shared_ptr<Attribute> AttributePtr;
class Closure;
typedef std::shared_ptr<Closure> ClosurePtr;
class Program;
typedef std::shared_ptr<Program> ProgramPtr;



class Import;
typedef std::shared_ptr<Import> ImportPtr;
typedef std::shared_ptr<class ValueBinding> ValueBindingPtr;
class Constant;
typedef std::shared_ptr<Constant> ConstantPtr;
class Constants;
typedef std::shared_ptr<Constants> ConstantsPtr;
class Variables;
typedef std::shared_ptr<Variables> VariablesPtr;
class Variable;
typedef std::shared_ptr<Variable> VariablePtr;
class TypeAlias;
typedef std::shared_ptr<TypeAlias> TypeAliasPtr;
class FunctionDef;
typedef std::shared_ptr<FunctionDef> FunctionDefPtr;
class EnumDef;
typedef std::shared_ptr<EnumDef> EnumDefPtr;
class StructDef;
typedef std::shared_ptr<StructDef> StructDefPtr;
class ClassDef;
typedef std::shared_ptr<ClassDef> ClassDefPtr;
class ProtocolDef;
typedef std::shared_ptr<ProtocolDef> ProtocolDefPtr;
class InitializerDef;
typedef std::shared_ptr<InitializerDef> InitializerDefPtr;
class DeinitializerDef;
typedef std::shared_ptr<DeinitializerDef> DeinitializerDefPtr;
class ExtensionDef;
typedef std::shared_ptr<ExtensionDef> ExtensionDefPtr;
class SubscriptDef;
typedef std::shared_ptr<SubscriptDef> SubscriptDefPtr;
class OperatorDef;
typedef std::shared_ptr<OperatorDef> OperatorDefPtr;
class Parameters;
typedef std::shared_ptr<Parameters> ParametersPtr;
class Parameter;
typedef std::shared_ptr<Parameter> ParameterPtr;
class GenericConstraint;
typedef std::shared_ptr<GenericConstraint> GenericConstraintPtr;
class GenericParameters;
typedef std::shared_ptr<GenericParameters> GenericParametersPtr;
class Pattern;
typedef std::shared_ptr<Pattern> PatternPtr;
class EnumCasePattern;
typedef std::shared_ptr<EnumCasePattern> EnumCasePatternPtr;

typedef std::vector<AttributePtr> Attributes;


SWIFT_NS_END

#endif//
