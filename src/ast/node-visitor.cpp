#include "node-visitor.h"
#include "ast/ast.h"
USE_SWIFT_NS


#define ACCEPT(node) do { \
        Node* n = node; \
        if(n) { \
            n->accept(this); \
        } \
    } while(0);


void NodeVisitor::visitDeinit(const DeinitializerDefPtr& node)
{
    ACCEPT(node->getBody().get());
}

void NodeVisitor::visitInit(const InitializerDefPtr& node)
{
    ACCEPT(node->getBody().get());
}

void NodeVisitor::visitImport(const ImportPtr& node)
{

}

void NodeVisitor::visitSubscript(const SubscriptDefPtr& node)
{
    ACCEPT(node->getGetter().get());
    ACCEPT(node->getSetter().get());
}

void NodeVisitor::visitTypeAlias(const TypeAliasPtr& node)
{
}

void NodeVisitor::visitForIn(const ForInLoopPtr& node)
{
    ACCEPT(node->getContainer().get());
    ACCEPT(node->getCodeBlock().get());
}

void NodeVisitor::visitForLoop(const ForLoopPtr& node)
{
    for(ExpressionPtr init : node->inits)
    {
        ACCEPT(init.get());
    }
    ACCEPT(node->condition.get());
    ACCEPT(node->step.get());
    ACCEPT(node->codeBlock.get());
}

void NodeVisitor::visitDoLoop(const DoLoopPtr& node)
{
    ACCEPT(node->getCodeBlock().get());
    ACCEPT(node->getCondition().get());
}

void NodeVisitor::visitLabeledStatement(const LabeledStatementPtr& node)
{
    ACCEPT(node->getStatement().get());
}

void NodeVisitor::visitOperator(const OperatorDefPtr& node)
{

}

void NodeVisitor::visitArrayLiteral(const ArrayLiteralPtr& node)
{
    for(ExpressionPtr expr : node->elements)
    {
        ACCEPT(expr.get());
    }
}

void NodeVisitor::visitDictionaryLiteral(const DictionaryLiteralPtr& node)
{
    for(auto entry : *node)
    {
        ACCEPT(entry.first.get());
        ACCEPT(entry.second.get());
    }
}

void NodeVisitor::visitBreak(const BreakStatementPtr& node)
{
}

void NodeVisitor::visitReturn(const ReturnStatementPtr& node)
{
    ACCEPT(node->getExpression().get());
}

void NodeVisitor::visitContinue(const ContinueStatementPtr& node)
{
}

void NodeVisitor::visitFallthrough(const FallthroughStatementPtr& node)
{
}

void NodeVisitor::visitIf(const IfStatementPtr& node)
{
    ACCEPT(node->getCondition().get());
    ACCEPT(node->getThen().get());
    ACCEPT(node->getElse().get());
}

void NodeVisitor::visitSwitchCase(const SwitchCasePtr& node)
{
    ACCEPT(node->getControlExpression().get());
    for(CaseStatementPtr c : *node)
    {
        ACCEPT(c.get());
    }
    ACCEPT(node->getDefaultCase().get());
}

void NodeVisitor::visitCase(const CaseStatementPtr& node)
{
    for(auto condition : node->getConditions())
    {
        ACCEPT(condition.condition.get());
        ACCEPT(condition.guard.get());
    }
    for(auto st : *node)
    {
        ACCEPT(st.get());
    }
}

void NodeVisitor::visitCodeBlock(const CodeBlockPtr& node)
{
    for(auto st: *node)
    {
        ACCEPT(st.get());
    }
}

void NodeVisitor::visitParameter(const ParameterPtr& node)
{

}

void NodeVisitor::visitParameters(const ParametersPtr& node)
{
    for(ParameterPtr p : *node)
    {
        ACCEPT(p.get());
    }
}

void NodeVisitor::visitProgram(const ProgramPtr& node)
{
    for(StatementPtr st : *node)
    {
        ACCEPT(st.get());
    }
}

void NodeVisitor::visitLetBinding(const LetBindingPtr& node)
{

}

void NodeVisitor::visitVarBinding(const VarBindingPtr& node)
{
}


void NodeVisitor::visitVariables(const VariablesPtr& node)
{
    for(VariablePtr var : *node)
    {
        ACCEPT(var.get());
    }
}

void NodeVisitor::visitVariable(const VariablePtr& node)
{
    ACCEPT(node->getInitializer().get());
    ACCEPT(node->getGetter().get());
    ACCEPT(node->getSetter().get());
    ACCEPT(node->getWillSet().get());
    ACCEPT(node->getDidSet().get());
}


void NodeVisitor::visitConstant(const ConstantPtr& node)
{
    ACCEPT(node->initializer.get());
}
void NodeVisitor::visitConstants(const ConstantsPtr& node)
{
    for(auto c : *node)
    {
        ACCEPT(c.get());
    }
}

void NodeVisitor::visitAssignment(const AssignmentPtr& node)
{
    ACCEPT(node->getLHS().get());
    ACCEPT(node->getRHS().get());
}

void NodeVisitor::visitClass(const ClassDefPtr& node)
{
    for(DeclarationPtr dec : *node)
    {
        dec->accept(this);
    }
}

void NodeVisitor::visitStruct(const StructDefPtr& node)
{
    for(DeclarationPtr dec : *node)
    {
        dec->accept(this);
    }
}

void NodeVisitor::visitEnum(const EnumDefPtr& node)
{
    for(DeclarationPtr dec : *node)
    {
        dec->accept(this);
    }
}

void NodeVisitor::visitProtocol(const ProtocolDefPtr& node)
{
    for(DeclarationPtr dec : *node)
    {
        dec->accept(this);
    }
}

void NodeVisitor::visitExtension(const ExtensionDefPtr& node)
{
    for(DeclarationPtr dec : *node)
    {
        dec->accept(this);
    }
}

void NodeVisitor::visitFunction(const FunctionDefPtr& node)
{
    for(ParametersPtr params : node->getParametersList())
    {
        ACCEPT(params.get());
    }
    ACCEPT(node->getBody().get());
}

void NodeVisitor::visitWhileLoop(const WhileLoopPtr& node)
{
    ACCEPT(node->getCondition().get());
    ACCEPT(node->getCodeBlock().get());
}

void NodeVisitor::visitConditionalOperator(const ConditionalOperatorPtr& node)
{
    ACCEPT(node->getCondition().get());
    ACCEPT(node->getTrueExpression().get());
    ACCEPT(node->getFalseExpression().get());
}

void NodeVisitor::visitBinaryOperator(const BinaryOperatorPtr& node)
{
    ACCEPT(node->getLHS().get());
    ACCEPT(node->getRHS().get());
}

void NodeVisitor::visitUnaryOperator(const UnaryOperatorPtr& node)
{
    ACCEPT(node->getOperand().get());
}

void NodeVisitor::visitTuple(const TuplePtr& node)
{
    for(const PatternPtr& p : *node)
    {
        ACCEPT(p.get());
    }
}

void NodeVisitor::visitIdentifier(const IdentifierPtr& node)
{

}

void NodeVisitor::visitCompileConstant(const CompileConstantPtr& node)
{
}

void NodeVisitor::visitSubscriptAccess(const SubscriptAccessPtr& node)
{
    ACCEPT(node->getSelf().get());
    for(ExpressionPtr index : *node)
    {
        ACCEPT(index.get());
    }
}

void NodeVisitor::visitMemberAccess(const MemberAccessPtr& node)
{
    ACCEPT(node->getSelf().get());
}

void NodeVisitor::visitFunctionCall(const FunctionCallPtr& node)
{
    ACCEPT(node->getFunction().get());
    ACCEPT(node->getArguments().get());
    ACCEPT(node->getTrailingClosure().get())
}

void NodeVisitor::visitClosure(const ClosurePtr& node)
{
    ACCEPT(node->getCapture().get());
    ACCEPT(node->getParameters().get());
    for(const StatementPtr& st : *node)
    {
        ACCEPT(st.get());
    }
}

void NodeVisitor::visitSelf(const SelfExpressionPtr& node)
{
    ACCEPT(node->getExpression().get());
}

void NodeVisitor::visitInitializerReference(const InitializerReferencePtr& node)
{
    ACCEPT(node->getExpression().get());
}


void NodeVisitor::visitEnumCasePattern(const EnumCasePatternPtr& node)
{
}

void NodeVisitor::visitDynamicType(const DynamicTypePtr& node)
{
    ACCEPT(node->getExpression().get());
}

void NodeVisitor::visitForcedValue(const ForcedValuePtr& node)
{
    ACCEPT(node->getExpression().get());
}

void NodeVisitor::visitOptionalChaining(const OptionalChainingPtr& node)
{
    ACCEPT(node->getExpression().get());
}

void NodeVisitor::visitParenthesizedExpression(const ParenthesizedExpressionPtr& node)
{
    for(auto term : *node)
    {
        ACCEPT(term.second.get());
    }
}
