/* NodeFactory.h --
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
#ifndef NODE_FACTORY_H
#define NODE_FACTORY_H
#include "swallow_types.h"
#include <string>
#include <vector>
#include "ast-decl.h"

SWALLOW_NS_BEGIN

struct SourceInfo;

class SWALLOW_EXPORT NodeFactory
{
public:
    NodeFactory();
    virtual ~NodeFactory(){}
public:
    virtual ProgramPtr createProgram();
    virtual CommentNodePtr createComment(const SourceInfo& state);
    virtual IntegerLiteralPtr createInteger(const SourceInfo& state);
    virtual FloatLiteralPtr createFloat(const SourceInfo& state);
    virtual StringLiteralPtr createString(const SourceInfo& state);
    virtual NilLiteralPtr createNilLiteral(const SourceInfo& state);
    virtual BooleanLiteralPtr createBooleanLiteral(const SourceInfo& state);
    virtual DefaultValuePtr createDefaultValue(const SourceInfo& state);
    virtual UnaryOperatorPtr createUnary(const SourceInfo& state);
    virtual BinaryOperatorPtr createBinary(const SourceInfo& state);

    virtual TypedPatternPtr createTypedPattern(const SourceInfo& state);
    virtual IdentifierPtr createIdentifier(const SourceInfo& state);
    virtual GenericArgumentDefPtr createGenericArgumentDef(const SourceInfo& state);
    virtual InOutParameterNode createInOutParameter(const SourceInfo& state);

    virtual ArrayLiteralPtr createArrayLiteral(const SourceInfo& state);
    virtual DictionaryLiteralPtr createDictionaryLiteral(const SourceInfo& state);
    virtual CompileConstantPtr createCompilecConstant(const SourceInfo& state);
    virtual MemberAccessPtr createMemberAccess(const SourceInfo& state);
    virtual SubscriptAccessPtr createSubscriptAccess(const SourceInfo& state);
    
    virtual TypeCheckPtr createTypeCheck(const SourceInfo& state);
    virtual TypeCastPtr createTypeCast(const SourceInfo& state);
    virtual AssignmentPtr createAssignment(const SourceInfo& state);
    virtual ConditionalOperatorPtr createConditionalOperator(const SourceInfo& state);
    virtual ParenthesizedExpressionPtr createParenthesizedExpression(const SourceInfo& state);
    
    virtual InitializerReferencePtr createInitializerReference(const SourceInfo& state);
    virtual SelfExpressionPtr createSelfExpression(const SourceInfo& state);
    virtual DynamicTypePtr createDynamicType(const SourceInfo& state);
    virtual ForcedValuePtr createForcedValue(const SourceInfo& state);
    virtual OptionalChainingPtr createOptionalChaining(const SourceInfo& state);
    virtual FunctionCallPtr createFunctionCall(const SourceInfo& state);

    virtual ForLoopPtr createForLoop(const SourceInfo& state);
    virtual ForInLoopPtr createForInLoop(const SourceInfo& state);
    virtual WhileLoopPtr createWhileLoop(const SourceInfo& state);
    virtual IfStatementPtr createIf(const SourceInfo& state);
    virtual DoLoopPtr createDoLoop(const SourceInfo& state);
    virtual SwitchCasePtr createSwitch(const SourceInfo& state);
    virtual CaseStatementPtr createCase(const SourceInfo& state);
    virtual BreakStatementPtr createBreak(const SourceInfo& state);
    virtual ContinueStatementPtr createContinue(const SourceInfo& state);
    virtual FallthroughStatementPtr createFallthrough(const SourceInfo& state);
    virtual ReturnStatementPtr createReturn(const SourceInfo& state);
    virtual LabeledStatementPtr createLabel(const SourceInfo& state);
    virtual CodeBlockPtr createCodeBlock(const SourceInfo& state);
    virtual ValueBindingPatternPtr createValueBindingPattern(const SourceInfo& state);
    virtual TuplePtr createTuple(const SourceInfo& state);
    virtual ClosurePtr createClosure(const SourceInfo& state);
    virtual EnumCasePatternPtr createEnumCasePattern(const SourceInfo& state);
    
    virtual FunctionTypePtr createFunctionType(const SourceInfo& state);
    virtual ArrayTypePtr createArrayType(const SourceInfo& state);
    virtual OptionalTypePtr createOptionalType(const SourceInfo& state);
    virtual ImplicitlyUnwrappedOptionalPtr createImplicitlyUnwrappedOptional(const SourceInfo& state);
    virtual TypeIdentifierPtr createTypeIdentifier(const SourceInfo& state);
    virtual ProtocolCompositionPtr createProtocolComposition(const SourceInfo& state);
    virtual TupleTypePtr createTupleType(const SourceInfo& state);
    virtual AttributePtr createAttribute(const SourceInfo& state);
    virtual DictionaryTypePtr createDictionaryType(const SourceInfo& state);

    
    
    virtual ImportPtr createImport(const SourceInfo& state);
    virtual ValueBindingPtr createValueBinding(const SourceInfo& state);
    virtual ValueBindingsPtr createValueBindings(const SourceInfo& state);
    virtual ComputedPropertyPtr createComputedProperty(const SourceInfo& state);
    virtual TypeAliasPtr createTypealias(const SourceInfo& state);
    virtual FunctionDefPtr createFunction(const SourceInfo& state);
    virtual ParametersNodePtr createParameters(const SourceInfo& state);
    virtual ParameterNodePtr createParameter(const SourceInfo& state);
    virtual EnumDefPtr createEnum(const SourceInfo& state);
    virtual StructDefPtr createStruct(const SourceInfo& state);
    virtual ClassDefPtr createClass(const SourceInfo& state);
    virtual ProtocolDefPtr createProtocol(const SourceInfo& state);
    virtual InitializerDefPtr createInitializer(const SourceInfo& state);
    virtual DeinitializerDefPtr createDeinitializer(const SourceInfo& state);
    virtual ExtensionDefPtr createExtension(const SourceInfo& state);
    virtual SubscriptDefPtr createSubscript(const SourceInfo& state);
    virtual OperatorDefPtr createOperator(const SourceInfo& state);
    virtual GenericConstraintDefPtr createGenericConstraintDef(const SourceInfo& state);
    virtual GenericParametersDefPtr createGenericParametersDef(const SourceInfo& state);
    virtual StringInterpolationPtr createStringInterpolation(const SourceInfo& state);
protected:

    void bindNode(const SourceInfo&s, Node* n);

    template<class T>
    inline std::shared_ptr<T> create(const SourceInfo& s)
    {
        std::shared_ptr<T> ret(new T());
        bindNode(s, ret.get());
        return ret;
    }

};


SWALLOW_NS_END


#endif//NODE_FACTORY_H
