/* WorkflowBranchValidator.h --
 *
 * Copyright (c) 2014, Lex Chou <lex at chou dot it>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Swallow nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef WORKFLOW_BRANCH_VALIDATOR_H
#define WORKFLOW_BRANCH_VALIDATOR_H
#include "ast/NodeVisitor.h"
#include <experimental/optional>

SWALLOW_NS_BEGIN

    struct SemanticContext;
    enum BranchCoverResult
    {
        BranchCoverNoResult = 0,
        /*!
         * No branch matched
         */
        BranchCoverUnmatched = 1,
        /*!
         * Partially matched
         */
        BranchCoverPartial = 3,
        /*!
         * All possible paths are covered
         */
        BranchCoverFull = 2,
        /*!
         * Exists more than once
         */
        BranchCoverMultiple = 4
    };
/*!
 * Validate return/init in all branches
 */
    class SWALLOW_EXPORT WorkflowBranchValidator : public NodeVisitor
    {
    private:
        enum MergeType
        {
            MergeSequence,
            MergeBranch
        };
    public:
        enum ValidateType
        {
            /*!
             * Validate super.init or self.init, make sure it exists in all possible branches
             */
            VALIDATE_INIT_CALL,
            /*!
             * Validate return statement, make sure return exists in all possible branches
             */
            VALIDATE_RETURN
        };
    public:
        WorkflowBranchValidator(SemanticContext* ctx, ValidateType validateType);
    public:
        virtual void visitWhileLoop(const WhileLoopPtr& node) override;
        virtual void visitForIn(const ForInLoopPtr& node) override;
        virtual void visitForLoop(const ForLoopPtr& node) override;
        virtual void visitDoLoop(const DoLoopPtr& node) override;
        virtual void visitLabeledStatement(const LabeledStatementPtr& node);
        virtual void visitBreak(const BreakStatementPtr& node) override;
        virtual void visitReturn(const ReturnStatementPtr& node) override;
        virtual void visitContinue(const ContinueStatementPtr& node) override;
        virtual void visitFallthrough(const FallthroughStatementPtr& node) override;
        virtual void visitIf(const IfStatementPtr& node) override;
        virtual void visitSwitchCase(const SwitchCasePtr& node) override ;
        virtual void visitCase(const CaseStatementPtr& node) override;
        virtual void visitCodeBlock(const CodeBlockPtr& node) override;
        virtual void visitFunctionCall(const FunctionCallPtr& node) override;
    public:
        BranchCoverResult getResult() const {return result;}
        NodePtr getRefNode() const { return refNode;}
    private:
        /*!
         * Merge the validator result to current instance.
         */
        void merge(WorkflowBranchValidator& validator, MergeType mergeType);
    private:
        SemanticContext* ctx;
        ValidateType validateType;
        BranchCoverResult result;
        NodePtr refNode;
    };

SWALLOW_NS_END

#endif//WORKFLOW_BRANCH_VALIDATOR_H
