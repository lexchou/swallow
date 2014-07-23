#include "node-visitor.h"
#include "ast/ast.h"
USE_SWIFT_NS


#define ACCEPT(node) do { \
        Node* n = node; \
        if(n) { \
            n->accept(this); \
        } \
    } while(0);


void NodeVisitor::visitDeinit(DeinitializerDef* node)
{
    ACCEPT(node->getBody());
}

void NodeVisitor::visitInit(InitializerDef* node)
{
    ACCEPT(node->getBody());
}

void NodeVisitor::visitImport(Import* node)
{

}

void NodeVisitor::visitSubscript(SubscriptDef* node)
{
    ACCEPT(node->getGetter());
    ACCEPT(node->getSetter());
}

void NodeVisitor::visitTypeAlias(TypeAlias* node)
{
}

void NodeVisitor::visitForIn(ForInLoop* node)
{
    ACCEPT(node->getContainer());
    ACCEPT(node->getCodeBlock());
}

void NodeVisitor::visitForLoop(ForLoop* node)
{
    for(Expression* init : node->inits)
    {
        ACCEPT(init);
    }
    ACCEPT(node->condition);
    ACCEPT(node->step);
    ACCEPT(node->codeBlock);
}

void NodeVisitor::visitDoLoop(DoLoop* node)
{
    ACCEPT(node->getCodeBlock());
    ACCEPT(node->getCondition());
}

void NodeVisitor::visitLabeledStatement(LabeledStatement* node)
{
    ACCEPT(node->getStatement());
}

void NodeVisitor::visitOperator(OperatorDef* node)
{

}

void NodeVisitor::visitArrayLiteral(ArrayLiteral* node)
{
    for(Expression* expr : node->elements)
    {
        ACCEPT(expr);
    }
}

void NodeVisitor::visitDictionaryLiteral(DictionaryLiteral* node)
{
    for(auto entry : *node)
    {
        ACCEPT(entry.first);
        ACCEPT(entry.second);
    }
}

void NodeVisitor::visitBreak(BreakStatement* node)
{
}

void NodeVisitor::visitReturn(ReturnStatement* node)
{
    ACCEPT(node->getExpression());
}

void NodeVisitor::visitContinue(ContinueStatement* node)
{
}

void NodeVisitor::visitFallthrough(FallthroughStatement* node)
{
}

void NodeVisitor::visitIf(IfStatement* node)
{
    ACCEPT(node->getCondition());
    ACCEPT(node->getThen());
    ACCEPT(node->getElse());
}

void NodeVisitor::visitSwitchCase(SwitchCase* node)
{
    ACCEPT(node->getControlExpression());
    for(CaseStatement* c : *node)
    {
        ACCEPT(c);
    }
    ACCEPT(node->getDefaultCase());
}

void NodeVisitor::visitCase(CaseStatement* node)
{
    for(auto condition : node->getConditions())
    {
        ACCEPT(condition.condition);
        ACCEPT(condition.guard);
    }
    for(auto st : *node)
    {
        ACCEPT(st);
    }
}

void NodeVisitor::visitCodeBlock(CodeBlock* node)
{
    for(auto st: *node)
    {
        ACCEPT(st);
    }
}

void NodeVisitor::visitParameter(Parameter* node)
{

}

void NodeVisitor::visitParameters(Parameters* node)
{
    for(Parameter* p : *node)
    {
        ACCEPT(p);
    }
}

void NodeVisitor::visitProgram(Program* node)
{
    for(Statement* st : *node)
    {
        ACCEPT(st);
    }
}

void NodeVisitor::visitLetBinding(LetBinding* node)
{

}

void NodeVisitor::visitVarBinding(VarBinding* node)
{
}


void NodeVisitor::visitVariables(Variables* node)
{
    for(Variable* var : *node)
    {
        ACCEPT(var);
    }
}

void NodeVisitor::visitVariable(Variable* node)
{
    ACCEPT(node->getInitializer());
    ACCEPT(node->getGetter());
    ACCEPT(node->getSetter());
    ACCEPT(node->getWillSet());
    ACCEPT(node->getDidSet());
}

void NodeVisitor::visitConstants(Constant* node)
{
    for(auto c : *node)
    {
        ACCEPT(c.second);
    }
}

void NodeVisitor::visitAssignment(Assignment* node)
{
    ACCEPT(node->getLHS());
    ACCEPT(node->getRHS();)
}

void NodeVisitor::visitClass(ClassDef* node)
{
    for(Declaration* dec : *node)
    {
        dec->accept(this);
    }
}

void NodeVisitor::visitStruct(StructDef* node)
{
    for(Declaration* dec : *node)
    {
        dec->accept(this);
    }
}

void NodeVisitor::visitEnum(EnumDef* node)
{
    for(Declaration* dec : *node)
    {
        dec->accept(this);
    }
}

void NodeVisitor::visitProtocol(ProtocolDef* node)
{
    for(Declaration* dec : *node)
    {
        dec->accept(this);
    }
}

void NodeVisitor::visitExtension(ExtensionDef* node)
{
    for(Declaration* dec : *node)
    {
        dec->accept(this);
    }
}

void NodeVisitor::visitFunction(FunctionDef* node)
{
    for(Parameters* params : node->getParametersList())
    {
        ACCEPT(params);
    }
    ACCEPT(node->getBody());
}

void NodeVisitor::visitWhileLoop(WhileLoop* node)
{
    ACCEPT(node->getCondition());
    ACCEPT(node->getCodeBlock());
}

void NodeVisitor::visitConditionalOperator(ConditionalOperator* node)
{
    ACCEPT(node->getCondition());
    ACCEPT(node->getTrueExpression());
    ACCEPT(node->getFalseExpression());
}

void NodeVisitor::visitBinaryOperator(BinaryOperator* node)
{
    ACCEPT(node->getLHS());
    ACCEPT(node->getRHS());
}

void NodeVisitor::visitUnaryOperator(UnaryOperator* node)
{
    ACCEPT(node->getOperand());
}

void NodeVisitor::visitTuple(Tuple* node)
{
    for(Pattern* p : *node)
    {
        ACCEPT(p);
    }
}

void NodeVisitor::visitIdentifier(Identifier* node)
{

}

void NodeVisitor::visitCompileConstant(CompileConstant* node)
{
}

void NodeVisitor::visitSubscriptAccess(SubscriptAccess* node)
{
    ACCEPT(node->getSelf());
    for(Expression* index : *node)
    {
        ACCEPT(index);
    }
}

void NodeVisitor::visitMemberAccess(MemberAccess* node)
{
    ACCEPT(node->getSelf());
}

void NodeVisitor::visitFunctionCall(FunctionCall* node)
{
    ACCEPT(node->getFunction());
    ACCEPT(node->getArguments());
    ACCEPT(node->getTrailingClosure())
}

void NodeVisitor::visitClosure(Closure* node)
{
    ACCEPT(node->getCapture());
    ACCEPT(node->getParameters());
    for(Statement* st : *node)
    {
        ACCEPT(st);
    }
}

void NodeVisitor::visitSelf(SelfExpression* node)
{
    ACCEPT(node->getExpression());
}

void NodeVisitor::visitInitializerReference(InitializerReference* node)
{
    ACCEPT(node->getExpression());
}


void NodeVisitor::visitEnumCasePattern(EnumCasePattern* node)
{
}

void NodeVisitor::visitDynamicType(DynamicType* node)
{
    ACCEPT(node->getExpression());
}

void NodeVisitor::visitForcedValue(ForcedValue* node)
{
    ACCEPT(node->getExpression());
}

void NodeVisitor::visitOptionalChaining(OptionalChaining* node)
{
    ACCEPT(node->getExpression());
}

void NodeVisitor::visitParenthesizedExpression(ParenthesizedExpression* node)
{
    for(auto term : *node)
    {
        ACCEPT(term.second);
    }
}
