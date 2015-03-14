#include "semantics/InitializerValidator.h"
#include "ast/ast.h"
#include "semantics/SemanticContext.h"
#include "semantics/CompilerResultEmitter.h"
#include "common/Errors.h"
USE_SWALLOW_NS
using namespace std;

InitializerValidator::InitializerValidator(SemanticContext* ctx, CompilerResultEmitter* compilerResults)
    :ctx(ctx), result(InitializerCoverNoResult), compilerResults(compilerResults)
{

}
InitializerValidator::InitializerValidator(InitializerValidator* validator)
    :ctx(validator->ctx), result(InitializerCoverNoResult), compilerResults(validator->compilerResults)
{

}
/*!
 * Merge the validator result to current instance.
 */
void InitializerValidator::merge(InitializerValidator& validator, MergeType mergeType)
{
    merge(mergeType, validator.result, validator.refNode);
}
void InitializerValidator::merge(MergeType mergeType, InitializerCoverResult newResult, const NodePtr& newRefNode)
{
    //sequence merge will check if the init/return exists more than once
    if(mergeType == MergeBranch && newResult == InitializerCoverNoResult)
        newResult = InitializerCoverUnmatched;
    if(mergeType == MergeSequence)
    {
        //this check only applies for super.init/self.init because calling initializer delegation
        //will not interrupt the workflow
        if ((result & InitializerCoverFull) && (newResult & InitializerCoverFull))
            result = (InitializerCoverResult) (result | InitializerCoverMultiple);
    }
    result = (InitializerCoverResult) (result | newResult);
    if(result & InitializerCoverMultiple && !refNode)
        refNode = newRefNode;

}
void InitializerValidator::visitFunctionCall(const FunctionCallPtr& node)
{
    if(node->getFunction()->getNodeType() != NodeType::MemberAccess)
        return;
    MemberAccessPtr ma = static_pointer_cast<MemberAccess>(node->getFunction());
    bool isSelf = false, isSuper = false;
    if(ma->getSelf() && ma->getSelf()->getNodeType() == NodeType::Identifier)
    {
        IdentifierPtr id = static_pointer_cast<Identifier>(ma->getSelf());
        isSelf = id->getIdentifier() == L"self";
        isSuper = id->getIdentifier() == L"super";
    }
    if(ma->getField() && ma->getField()->getIdentifier() == L"init")
    {
        merge(MergeSequence, InitializerCoverFull, node);
    }
}

void InitializerValidator::visitWhileLoop(const WhileLoopPtr& node)
{
    InitializerValidator validator(this);
    node->getCodeBlock()->accept(&validator);
    merge(validator, MergeSequence);

}
void InitializerValidator::visitForIn(const ForInLoopPtr& node)
{
    InitializerValidator validator(this);
    node->getCodeBlock()->accept(&validator);
    merge(validator, MergeSequence);
}
void InitializerValidator::visitForLoop(const ForLoopPtr& node)
{

    InitializerValidator validator(this);
    node->getCodeBlock()->accept(&validator);
    merge(validator, MergeSequence);
}
void InitializerValidator::visitDoLoop(const DoLoopPtr& node)
{

    InitializerValidator validator(this);
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
    InitializerValidator validator(this);

    InitializerValidator thenBranch(this);
    node->getThen()->accept(&thenBranch);
    validator.merge(thenBranch, MergeBranch);

    InitializerValidator elseBranch(this);
    if(node->getElse())
        node->getElse()->accept(&elseBranch);
    validator.merge(elseBranch, MergeBranch);

    merge(validator, MergeSequence);
}
void InitializerValidator::visitSwitchCase(const SwitchCasePtr& node)
{
    InitializerValidator validator(this);
    for(const CaseStatementPtr& c : * node)
    {
        InitializerValidator caseBranch(this);
        c->accept(&caseBranch);
        validator.merge(caseBranch, MergeBranch);
    }
    InitializerValidator defaultCase(this);
    if(node->getDefaultCase())
    {
        node->getDefaultCase()->accept(&defaultCase);
    }
    validator.merge(defaultCase, MergeBranch);
    merge(validator, MergeSequence);
}
void InitializerValidator::visitCase(const CaseStatementPtr& node)
{
    node->getCodeBlock()->accept(this);
}
void InitializerValidator::visitCodeBlock(const CodeBlockPtr& node)
{
    for(const StatementPtr& st : * node)
    {
        st->accept(this);
    }
}
