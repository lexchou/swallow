#include "node-visitor-decorator.h"
USE_SWIFT_NS


#define VISIT(fn) visitor->fn(node);
NodeVisitorDecorator::NodeVisitorDecorator(NodeVisitor* visitor)
:visitor(visitor)
{

}

void NodeVisitorDecorator::visitDeinit(DeinitializerDef* node)
{
    visitor->visitDeinit(node);
}

void NodeVisitorDecorator::visitInit(InitializerDef* node)
{
    visitor->visitInit(node);
}

void NodeVisitorDecorator::visitImport(Import* node)
{
    visitor->visitImport(node);
}

void NodeVisitorDecorator::visitSubscript(SubscriptDef* node)
{
    visitor->visitSubscript(node);
}

void NodeVisitorDecorator::visitTypeAlias(TypeAlias* node)
{
    visitor->visitTypeAlias(node);
}

void NodeVisitorDecorator::visitForIn(ForInLoop* node)
{
    visitor->visitForIn(node);
}

void NodeVisitorDecorator::visitForLoop(ForLoop* node)
{
    visitor->visitForLoop(node);
}

void NodeVisitorDecorator::visitDoLoop(DoLoop* node)
{
    visitor->visitDoLoop(node);
}

void NodeVisitorDecorator::visitLabeledStatement(LabeledStatement* node)
{
    visitor->visitLabeledStatement(node);
}

void NodeVisitorDecorator::visitOperator(OperatorDef* node)
{
    visitor->visitOperator(node);
}

void NodeVisitorDecorator::visitArrayLiteral(ArrayLiteral* node)
{
    visitor->visitArrayLiteral(node);
}

void NodeVisitorDecorator::visitDictionaryLiteral(DictionaryLiteral* node)
{
    visitor->visitDictionaryLiteral(node);
}

void NodeVisitorDecorator::visitBreak(BreakStatement* node)
{
    visitor->visitBreak(node);
}

void NodeVisitorDecorator::visitReturn(ReturnStatement* node)
{
    visitor->visitReturn(node);
}

void NodeVisitorDecorator::visitContinue(ContinueStatement* node)
{
    visitor->visitContinue(node);
}

void NodeVisitorDecorator::visitFallthrough(FallthroughStatement* node)
{
    visitor->visitFallthrough(node);
}

void NodeVisitorDecorator::visitIf(IfStatement* node)
{
    visitor->visitIf(node);
}

void NodeVisitorDecorator::visitSwitchCase(SwitchCase* node)
{
    visitor->visitSwitchCase(node);
}

void NodeVisitorDecorator::visitCase(CaseStatement* node)
{
    visitor->visitCase(node);
}

void NodeVisitorDecorator::visitCodeBlock(CodeBlock* node)
{
    visitor->visitCodeBlock(node);
}

void NodeVisitorDecorator::visitParameter(Parameter* node)
{
    VISIT(visitParameter);
}

void NodeVisitorDecorator::visitParameters(Parameters* node)
{
    VISIT(visitParameters);
}

void NodeVisitorDecorator::visitProgram(Program* node)
{
    VISIT(visitProgram);
}

void NodeVisitorDecorator::visitLetBinding(LetBinding* node)
{
    VISIT(visitLetBinding);
}

void NodeVisitorDecorator::visitVarBinding(VarBinding* node)
{
    VISIT(visitVarBinding);
}


void NodeVisitorDecorator::visitVariables(Variables* node)
{
    VISIT(visitVariables);
}

void NodeVisitorDecorator::visitVariable(Variable* node)
{
    VISIT(visitVariable);
}

void NodeVisitorDecorator::visitConstants(Constant* node)
{
    VISIT(visitConstants);
}

void NodeVisitorDecorator::visitAssignment(Assignment* node)
{
    VISIT(visitAssignment);
}

void NodeVisitorDecorator::visitClass(ClassDef* node)
{
    VISIT(visitClass);
}

void NodeVisitorDecorator::visitStruct(StructDef* node)
{
    VISIT(visitStruct);
}

void NodeVisitorDecorator::visitEnum(EnumDef* node)
{
    VISIT(visitEnum);
}

void NodeVisitorDecorator::visitProtocol(ProtocolDef* node)
{
    VISIT(visitProtocol);
}

void NodeVisitorDecorator::visitExtension(ExtensionDef* node)
{
    VISIT(visitExtension);
}

void NodeVisitorDecorator::visitFunction(FunctionDef* node)
{
    VISIT(visitFunction);
}

void NodeVisitorDecorator::visitWhileLoop(WhileLoop* node)
{
    VISIT(visitWhileLoop);
}

void NodeVisitorDecorator::visitConditionalOperator(ConditionalOperator* node)
{
    VISIT(visitConditionalOperator);
}

void NodeVisitorDecorator::visitBinaryOperator(BinaryOperator* node)
{
    VISIT(visitBinaryOperator);
}

void NodeVisitorDecorator::visitUnaryOperator(UnaryOperator* node)
{
    VISIT(visitUnaryOperator);
}

void NodeVisitorDecorator::visitTuple(Tuple* node)
{
    VISIT(visitTuple);
}

void NodeVisitorDecorator::visitIdentifier(Identifier* node)
{
    VISIT(visitIdentifier);
}

void NodeVisitorDecorator::visitCompileConstant(CompileConstant* node)
{
    VISIT(visitCompileConstant);
}

void NodeVisitorDecorator::visitSubscriptAccess(SubscriptAccess* node)
{
    VISIT(visitSubscriptAccess);
}

void NodeVisitorDecorator::visitMemberAccess(MemberAccess* node)
{
    VISIT(visitMemberAccess);
}

void NodeVisitorDecorator::visitFunctionCall(FunctionCall* node)
{
    VISIT(visitFunctionCall);
}

void NodeVisitorDecorator::visitClosure(Closure* node)
{
    VISIT(visitClosure);
}

void NodeVisitorDecorator::visitSelf(SelfExpression* node)
{
    VISIT(visitSelf);
}

void NodeVisitorDecorator::visitInitializerReference(InitializerReference* node)
{
    VISIT(visitInitializerReference);
}


void NodeVisitorDecorator::visitEnumCasePattern(EnumCasePattern* node)
{
    VISIT(visitEnumCasePattern);
}

void NodeVisitorDecorator::visitDynamicType(DynamicType* node)
{
    VISIT(visitDynamicType);
}

void NodeVisitorDecorator::visitForcedValue(ForcedValue* node)
{
    VISIT(visitForcedValue);
}

void NodeVisitorDecorator::visitOptionalChaining(OptionalChaining* node)
{
    VISIT(visitOptionalChaining);
}

void NodeVisitorDecorator::visitParenthesizedExpression(ParenthesizedExpression* node)
{
    VISIT(visitParenthesizedExpression);
}
