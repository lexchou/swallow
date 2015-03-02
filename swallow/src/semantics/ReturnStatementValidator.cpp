#include "semantics/ReturnStatementValidator.h"
#include "ast/ast.h"
USE_SWALLOW_NS
using namespace std;

ReturnStatementValidator::ReturnStatementValidator(SemanticContext* ctx)
:ctx(ctx), result(ReturnCoverNoResult)
{

}
/*!
 * Merge the validator result to current instance.
 */
void ReturnStatementValidator::merge(ReturnStatementValidator& validator, MergeType mergeType)
{
    //sequence merge will check if the init/return exists more than once
    ReturnCoverResult validatorResult = validator.result;
    if(mergeType == MergeBranch && validatorResult == ReturnCoverNoResult)
        validatorResult = ReturnCoverUnmatched;
    if(mergeType == MergeSequence)
    {
		//in sequence return validation mode, the full cover result will override previous result
		if(validatorResult == ReturnCoverFull)
			result = validatorResult;
    }
    result = (ReturnCoverResult) (result | validatorResult);
    if(result & ReturnCoverDeadcode && !refNode)
        refNode = validator.refNode;
}
void ReturnStatementValidator::visitReturn(const ReturnStatementPtr& node)
{
    result = (ReturnCoverResult)(result | ReturnCoverFull);
    refNode = node;
}
void ReturnStatementValidator::visitFunctionCall(const FunctionCallPtr& node)
{
	//TODO: check function that marked with @noreturn or @exit
}

void ReturnStatementValidator::visitWhileLoop(const WhileLoopPtr& node)
{
    ReturnStatementValidator validator(ctx);
    node->getCodeBlock()->accept(&validator);
    merge(validator, MergeSequence);

}
void ReturnStatementValidator::visitForIn(const ForInLoopPtr& node)
{
    ReturnStatementValidator validator(ctx);
    node->getCodeBlock()->accept(&validator);
    merge(validator, MergeSequence);
}
void ReturnStatementValidator::visitForLoop(const ForLoopPtr& node)
{

    ReturnStatementValidator validator(ctx);
    node->getCodeBlock()->accept(&validator);
    merge(validator, MergeSequence);
}
void ReturnStatementValidator::visitDoLoop(const DoLoopPtr& node)
{

    ReturnStatementValidator validator(ctx);
    node->getCodeBlock()->accept(&validator);
    merge(validator, MergeSequence);
}
void ReturnStatementValidator::visitLabeledStatement(const LabeledStatementPtr& node)
{
}
void ReturnStatementValidator::visitBreak(const BreakStatementPtr& node)
{

}
void ReturnStatementValidator::visitContinue(const ContinueStatementPtr& node)
{

}
void ReturnStatementValidator::visitFallthrough(const FallthroughStatementPtr& node)
{

}
void ReturnStatementValidator::visitIf(const IfStatementPtr& node)
{
    ReturnStatementValidator thenBranch(ctx);
    node->getThen()->accept(&thenBranch);
    merge(thenBranch, MergeBranch);

    ReturnStatementValidator elseBranch(ctx);
    if(node->getElse())
        node->getElse()->accept(&elseBranch);
    merge(elseBranch, MergeBranch);
}
void ReturnStatementValidator::visitSwitchCase(const SwitchCasePtr& node)
{
    for(const CaseStatementPtr& c : * node)
    {
        ReturnStatementValidator caseBranch(ctx);
        c->accept(&caseBranch);
        merge(caseBranch, MergeBranch);
    }
    ReturnStatementValidator defaultCase(ctx);
    if(node->getDefaultCase())
    {
        node->getDefaultCase()->accept(&defaultCase);
    }
    merge(defaultCase, MergeBranch);
}
void ReturnStatementValidator::visitCase(const CaseStatementPtr& node)
{
    node->getCodeBlock()->accept(this);
}
void ReturnStatementValidator::visitCodeBlock(const CodeBlockPtr& node)
{
    for(const StatementPtr& st : * node)
    {
		if (result == ReturnCoverFull)
		{
			result = ReturnCoverDeadcode;
			refNode = st;
			return;
		}
        ReturnStatementValidator validator(ctx);
        st->accept(&validator);
        merge(validator, MergeSequence);
    }
}
