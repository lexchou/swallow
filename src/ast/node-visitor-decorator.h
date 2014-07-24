#ifndef NODE_VISITOR_DECORATOR_H
#define NODE_VISITOR_DECORATOR_H
#include "node-visitor.h"

SWIFT_NS_BEGIN

class Statement;
class NodeVisitorDecorator : public NodeVisitor
{
public:
    NodeVisitorDecorator(NodeVisitor* visitor);
public:
    virtual void visitVariables(Variables* node);
    virtual void visitVariable(Variable* node);
    virtual void visitConstant(Constant* node);
    virtual void visitConstants(Constants* node);
    virtual void visitAssignment(Assignment* node);
    virtual void visitClass(ClassDef* node);
    virtual void visitStruct(StructDef* node);
    virtual void visitEnum(EnumDef* node);
    virtual void visitProtocol(ProtocolDef* node);
    virtual void visitExtension(ExtensionDef* node);
    virtual void visitFunction(FunctionDef* node);
    virtual void visitDeinit(DeinitializerDef* node);
    virtual void visitInit(InitializerDef* node);
    virtual void visitImport(Import* node);
    virtual void visitSubscript(SubscriptDef* node);
    virtual void visitTypeAlias(TypeAlias* node);
public://statement
    virtual void visitWhileLoop(WhileLoop* node);
    virtual void visitForIn(ForInLoop* node);
    virtual void visitForLoop(ForLoop* node);
    virtual void visitDoLoop(DoLoop* node);
    virtual void visitLabeledStatement(LabeledStatement* node);
    virtual void visitOperator(OperatorDef* node);
    virtual void visitArrayLiteral(ArrayLiteral* node);
    virtual void visitDictionaryLiteral(DictionaryLiteral* node);
    virtual void visitBreak(BreakStatement* node);
    virtual void visitReturn(ReturnStatement* node);
    virtual void visitContinue(ContinueStatement* node);
    virtual void visitFallthrough(FallthroughStatement* node);
    virtual void visitIf(IfStatement* node);
    virtual void visitSwitchCase(SwitchCase* node);
    virtual void visitCase(CaseStatement* node);
    virtual void visitCodeBlock(CodeBlock* node);
    virtual void visitParameter(Parameter* node);
    virtual void visitParameters(Parameters* node);
    virtual void visitProgram(Program* node);
    virtual void visitLetBinding(LetBinding* node);
    virtual void visitVarBinding(VarBinding* node);
public://The following visit procedures will not be executed until manually visit them by acceptPattern
    virtual void visitConditionalOperator(ConditionalOperator* node);
    virtual void visitBinaryOperator(BinaryOperator* node);
    virtual void visitUnaryOperator(UnaryOperator* node);
    virtual void visitTuple(Tuple* node);
    virtual void visitIdentifier(Identifier* node);
    virtual void visitCompileConstant(CompileConstant* node);
    virtual void visitSubscriptAccess(SubscriptAccess* node);
    virtual void visitMemberAccess(MemberAccess* node);
    virtual void visitFunctionCall(FunctionCall* node);
    virtual void visitClosure(Closure* node);
    virtual void visitSelf(SelfExpression* node);
    virtual void visitInitializerReference(InitializerReference* node);

    virtual void visitEnumCasePattern(EnumCasePattern* node);
    virtual void visitDynamicType(DynamicType* node);
    virtual void visitForcedValue(ForcedValue* node);
    virtual void visitOptionalChaining(OptionalChaining* node);
    virtual void visitParenthesizedExpression(ParenthesizedExpression* node);
private:
    NodeVisitor* visitor;
};


SWIFT_NS_END
#endif//NODE_VISITOR_DECORATOR_H