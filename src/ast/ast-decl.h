#ifndef AST_DECL_H
#define AST_DECL_H
#include "swift_conf.h"

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
class Program;



class Import;
class Constant;
class Constants;
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

SWIFT_NS_END

#endif//