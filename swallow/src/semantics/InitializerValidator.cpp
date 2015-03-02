#include "semantics/InitializerValidator.h"
#include "ast/ast.h"
USE_SWALLOW_NS
using namespace std;

InitializerValidator::InitializerValidator(SemanticContext* ctx)
:ctx(ctx), result(InitializerCoverNoResult)
{

}
/*!
 * Merge the validator result to current instance.
 */
void InitializerValidator::merge(InitializerValidator& validator, MergeType mergeType)
{
    //sequence merge will check if the init/return exists more than once
    InitializerCoverResult validatorResult = validator.result;
    if(mergeType == MergeBranch && validatorResult == InitializerCoverNoResult)
        validatorResult = InitializerCoverUnmatched;
    if(mergeType == MergeSequence)
    {
        //this check only applies for super.init/self.init because calling initializer delegation
        //will not interrupt the workflow
        if ((result & InitializerCoverFull) && (validator.result & InitializerCoverFull))
            result = (InitializerCoverResult) (result | InitializerCoverMultiple);
    }
    result = (InitializerCoverResult) (result | validatorResult);
    if(result & InitializerCoverMultiple && !refNode)
        refNode = validator.refNode;
}
void InitializerValidator::visitFunctionCall(const FunctionCallPtr& node)
{
    if(node->getFunction()->getNodeType() != NodeType::MemberAccess)
        return;
    MemberAccessPtr ma = static_pointer_cast<MemberAccess>(node->getFunction());
    if(ma->getField() && ma->getField()->getIdentifier() == L"init")
    {
        result = (InitializerCoverResult)(result | InitializerCoverFull);
        refNode = node;
    }
}

void InitializerValidator::visitWhileLoop(const WhileLoopPtr& node)
{
    InitializerValidator validator(ctx);
    node->getCodeBlock()->accept(&validator);
    merge(validator, MergeSequence);

}
void InitializerValidator::visitForIn(const ForInLoopPtr& node)
{
    InitializerValidator validator(ctx);
    node->getCodeBlock()->accept(&validator);
    merge(validator, MergeSequence);
}
void InitializerValidator::visitForLoop(const ForLoopPtr& node)
{

    InitializerValidator validator(ctx);
    node->getCodeBlock()->accept(&validator);
    merge(validator, MergeSequence);
}
void InitializerValidator::visitDoLoop(const DoLoopPtr& node)
{

    InitializerValidator validator(ctx);
    node->getCodeBlock()->accept(&validator);
    merge(validator, MergeSequence);
}
void InitializerValidator::visitLabeledStatement(const LabeledStatementPtr& node)
{
}
void InitializerValidator::visitBreak(const BreakStatementPtr& node)
{

}
void InitializerValidator::visitContinue(const ContinueStatementPtr& node)
{

}
void InitializerValidator::visitFallthrough(const FallthroughStatementPtr& node)
{

}
void InitializerValidator::visitIf(const IfStatementPtr& node)
{
    InitializerValidator thenBranch(ctx);
    node->getThen()->accept(&thenBranch);
    merge(thenBranch, MergeBranch);

    InitializerValidator elseBranch(ctx);
    if(node->getElse())
        node->getElse()->accept(&elseBranch);
    merge(elseBranch, MergeBranch);
}
void InitializerValidator::visitSwitchCase(const SwitchCasePtr& node)
{
    for(const CaseStatementPtr& c : * node)
    {
        InitializerValidator caseBranch(ctx);
        c->accept(&caseBranch);
        merge(caseBranch, MergeBranch);
    }
    InitializerValidator defaultCase(ctx);
    if(node->getDefaultCase())
    {
        node->getDefaultCase()->accept(&defaultCase);
    }
    merge(defaultCase, MergeBranch);
}
void InitializerValidator::visitCase(const CaseStatementPtr& node)
{
    node->getCodeBlock()->accept(this);
}
void InitializerValidator::visitCodeBlock(const CodeBlockPtr& node)
{
    for(const StatementPtr& st : * node)
    {
        InitializerValidator validator(ctx);
        st->accept(&validator);
        merge(validator, MergeSequence);
    }
}
