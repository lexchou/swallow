#ifndef NODE_VISITOR_H
#define NODE_VISITOR_H
#include "swift_conf.h"

SWIFT_NS_BEGIN

class Variables;
class Constant;
class Assignment;
class ClassDef;
class StructDef;
class EnumDef;
class ProtocolDef;
class ExtensionDef;
class FunctionDef;
class Pattern;
class BinaryOperator;
class Identifier;
class UnaryOperator;
class Tuple;
class CompileConstant;
class ConditionalOperator;
class FunctionCall;
class Closure;
class SelfExpression;
class SubscriptAccess;
class MemberAccess;
class InitializerReference;
class Statement;
class EnumCasePattern;
class DynamicType;
class ForcedValue;
class OptionalChaining;
class ParenthesizedExpression;

class NodeVisitor
{
public:
    virtual ~NodeVisitor(){}
public:
    virtual void visitVariables(Variables* node){}
    virtual void visitConstants(Constant* node){}
    virtual void visitAssignment(Assignment* node){}
    virtual void visitClass(ClassDef* node){}
    virtual void visitStruct(StructDef* node){}
    virtual void visitEnum(EnumDef* node){}
    virtual void visitProtocol(ProtocolDef* node){}
    virtual void visitExtension(ExtensionDef* node){}
    virtual void visitFunction(FunctionDef* node){}

public://The following visit procedures will not be executed until manually visit them by acceptPattern
    static void acceptPattern(Pattern* pattern, NodeVisitor* visitor);
    virtual void visitConditionalOperator(ConditionalOperator* node){}
    virtual void visitBinaryOperator(BinaryOperator* node){}
    virtual void visitUnaryOperator(UnaryOperator* node){}
    virtual void visitTuple(Tuple* node){}
    virtual void visitIdentifier(Identifier* node){}
    virtual void visitCompileConstant(CompileConstant* node){}
    virtual void visitStatement(Statement* st){}
    virtual void visitSubscriptAccess(SubscriptAccess* node){}
    virtual void visitMemberAccess(MemberAccess* node){}
    virtual void visitFunctionCall(FunctionCall* node){}
    virtual void visitClosure(Closure* node){}
    virtual void visitSelf(SelfExpression* node){}
    virtual void visitInitializerReference(InitializerReference* node){}

    virtual void visitEnumCasePattern(EnumCasePattern* node){}
    virtual void visitDynamicType(DynamicType* node){}
    virtual void visitForcedValue(ForcedValue* node){}
    virtual void visitOptionalChaining(OptionalChaining* node){}
    virtual void visitParenthesizedExpression(ParenthesizedExpression* node){}
};


SWIFT_NS_END
#endif//NODE_VISITOR_H