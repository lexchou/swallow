#include "semantics/WorkflowBranchValidator.h"
#include "ast/ast.h"
USE_SWALLOW_NS
using namespace std;

WorkflowBranchValidator::WorkflowBranchValidator(SemanticContext* ctx, WorkflowBranchValidator::ValidateType validateType)
:ctx(ctx), validateType(validateType), result(BranchCoverNoResult)
{

}
/*!
 * Merge the validator result to current instance.
 */
void WorkflowBranchValidator::merge(WorkflowBranchValidator& validator, MergeType mergeType)
{
    //sequence merge will check if the init/return exists more than once
    BranchCoverResult validatorResult = validator.result;
    if(mergeType == MergeBranch && validatorResult == BranchCoverNoResult)
        validatorResult = BranchCoverUnmatched;
    if(mergeType == MergeSequence)
    {
        if(validateType == VALIDATE_INIT_CALL)
        {
            //this check only applies for super.init/self.init because calling initializer delegation
            //will not interrupt the workflow
            if ((result & BranchCoverFull) && (validator.result & BranchCoverFull))
                result = (BranchCoverResult) (result | BranchCoverMultiple);
        }
        else
        {
            //in sequence return validation mode, the full cover result will override previous result
            if(validatorResult == BranchCoverFull)
                result = validatorResult;
        }
    }
    result = (BranchCoverResult) (result | validatorResult);
    if(result & BranchCoverMultiple && !refNode)
        refNode = validator.refNode;
}
void WorkflowBranchValidator::visitReturn(const ReturnStatementPtr& node)
{
    if(validateType != VALIDATE_RETURN)
        return;
    result = (BranchCoverResult)(result | BranchCoverFull);
    refNode = node;
}
void WorkflowBranchValidator::visitFunctionCall(const FunctionCallPtr& node)
{
    if(validateType != VALIDATE_INIT_CALL)
        return;
    if(node->getFunction()->getNodeType() != NodeType::MemberAccess)
        return;
    MemberAccessPtr ma = static_pointer_cast<MemberAccess>(node->getFunction());
    if(ma->getField() && ma->getField()->getIdentifier() == L"init")
    {
        result = (BranchCoverResult)(result | BranchCoverFull);
        refNode = node;
    }
}

void WorkflowBranchValidator::visitWhileLoop(const WhileLoopPtr& node)
{
    WorkflowBranchValidator validator(ctx, validateType);
    node->getCodeBlock()->accept(&validator);
    merge(validator, MergeSequence);

}
void WorkflowBranchValidator::visitForIn(const ForInLoopPtr& node)
{
    WorkflowBranchValidator validator(ctx, validateType);
    node->getCodeBlock()->accept(&validator);
    merge(validator, MergeSequence);
}
void WorkflowBranchValidator::visitForLoop(const ForLoopPtr& node)
{

    WorkflowBranchValidator validator(ctx, validateType);
    node->getCodeBlock()->accept(&validator);
    merge(validator, MergeSequence);
}
void WorkflowBranchValidator::visitDoLoop(const DoLoopPtr& node)
{

    WorkflowBranchValidator validator(ctx, validateType);
    node->getCodeBlock()->accept(&validator);
    merge(validator, MergeSequence);
}
void WorkflowBranchValidator::visitLabeledStatement(const LabeledStatementPtr& node)
{
}
void WorkflowBranchValidator::visitBreak(const BreakStatementPtr& node)
{

}
void WorkflowBranchValidator::visitContinue(const ContinueStatementPtr& node)
{

}
void WorkflowBranchValidator::visitFallthrough(const FallthroughStatementPtr& node)
{

}
void WorkflowBranchValidator::visitIf(const IfStatementPtr& node)
{
    WorkflowBranchValidator thenBranch(ctx, validateType);
    node->getThen()->accept(&thenBranch);
    merge(thenBranch, MergeBranch);

    WorkflowBranchValidator elseBranch(ctx, validateType);
    if(node->getElse())
        node->getElse()->accept(&elseBranch);
    merge(elseBranch, MergeBranch);
}
void WorkflowBranchValidator::visitSwitchCase(const SwitchCasePtr& node)
{
    for(const CaseStatementPtr& c : * node)
    {
        WorkflowBranchValidator caseBranch(ctx, validateType);
        c->accept(&caseBranch);
        merge(caseBranch, MergeBranch);
    }
    WorkflowBranchValidator defaultCase(ctx, validateType);
    if(node->getDefaultCase())
    {
        node->getDefaultCase()->accept(&defaultCase);
    }
    merge(defaultCase, MergeBranch);
}
void WorkflowBranchValidator::visitCase(const CaseStatementPtr& node)
{
    node->getCodeBlock()->accept(this);
}
void WorkflowBranchValidator::visitCodeBlock(const CodeBlockPtr& node)
{
    for(const StatementPtr& st : * node)
    {
        if(validateType == VALIDATE_RETURN)
        {
            if (result == BranchCoverFull)
            {
                result = BranchCoverMultiple;
                refNode = st;
                return;
            }
        }
        WorkflowBranchValidator validator(ctx, validateType);
        st->accept(&validator);
        merge(validator, MergeSequence);
    }
}
