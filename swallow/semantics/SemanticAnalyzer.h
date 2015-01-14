/* SemanticAnalyzer.h --
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
#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H
#include "SemanticPass.h"
#include "Type.h"
#include <list>

SWALLOW_NS_BEGIN

class TypeDeclaration;
class Expression;
class Pattern;
class NodeFactory;
typedef std::shared_ptr<class Identifier> IdentifierPtr;
struct TupleExtractionResult
{
    IdentifierPtr name;
    TypePtr type;
    ExpressionPtr initializer;
    bool readonly;

    TupleExtractionResult(const IdentifierPtr& name, const TypePtr& type, const ExpressionPtr& initializer, bool readonly)
         :name(name), type(type), initializer(initializer), readonly(readonly)
    {

    }
};
enum PatternAccessibility
{
    AccessibilityUndefined,
    AccessibilityConstant,
    AccessibilityVariable
};
class SWALLOW_EXPORT SemanticAnalyzer : public SemanticPass
{
public:
    SemanticAnalyzer(SymbolRegistry* symbolRegistry, CompilerResults* compilerResults);
public:
    virtual void visitAssignment(const AssignmentPtr& node) override;
    virtual void visitComputedProperty(const ComputedPropertyPtr& node) override;
    virtual void visitValueBindings(const ValueBindingsPtr& node) override;
    virtual void visitTypeAlias(const TypeAliasPtr& node) override;
    virtual void visitClass(const ClassDefPtr& node) override;
    virtual void visitStruct(const StructDefPtr& node) override;
    virtual void visitEnum(const EnumDefPtr& node) override;
    virtual void visitProtocol(const ProtocolDefPtr& node) override;
    virtual void visitExtension(const ExtensionDefPtr& node) override;
    virtual void visitFunction(const FunctionDefPtr& node) override;
    virtual void visitSubscript(const SubscriptDefPtr& node);
    virtual void visitClosure(const ClosurePtr& node) override;
    virtual void visitIdentifier(const IdentifierPtr& id) override;
    virtual void visitEnumCasePattern(const EnumCasePatternPtr& node) override;
    virtual void visitParameter(const ParameterPtr& node) override;
    virtual void visitDeinit(const DeinitializerDefPtr& node) override;
    virtual void visitInit(const InitializerDefPtr& node) override;
    virtual void visitCodeBlock(const CodeBlockPtr& node) override;
    virtual void visitParameters(const ParametersPtr& node) override;
public:

    virtual void visitFunctionCall(const FunctionCallPtr& node) override;
    virtual void visitValueBinding(const ValueBindingPtr& node) override;
    virtual void visitValueBindingPattern(const ValueBindingPatternPtr& node) override;
    virtual void visitString(const StringLiteralPtr& node) override;
    virtual void visitStringInterpolation(const StringInterpolationPtr& node);
    virtual void visitInteger(const IntegerLiteralPtr& node) override;
    virtual void visitFloat(const FloatLiteralPtr& node) override;
    virtual void visitNilLiteral(const NilLiteralPtr& node) override;
    virtual void visitBooleanLiteral(const BooleanLiteralPtr& node) override;
    virtual void visitConditionalOperator(const ConditionalOperatorPtr& node) override;
    virtual void visitBinaryOperator(const BinaryOperatorPtr& node) override;
    virtual void visitUnaryOperator(const UnaryOperatorPtr& node) override;
    virtual void visitTuple(const TuplePtr& node) override;
    //virtual void visitIdentifier(const IdentifierPtr& node) override;
    virtual void visitCompileConstant(const CompileConstantPtr& node) override;
    virtual void visitMemberAccess(const MemberAccessPtr& node) override;
    //virtual void visitClosure(const ClosurePtr& node) override;
    virtual void visitReturn(const ReturnStatementPtr& node) override;
    virtual void visitParenthesizedExpression(const ParenthesizedExpressionPtr& node) override;
    virtual void visitArrayLiteral(const ArrayLiteralPtr& node) override;
    virtual void visitDictionaryLiteral(const DictionaryLiteralPtr& node) override;
    //virtual void visitComputedProperty(const ComputedPropertyPtr& node) override;
    //virtual void visitSubscript(const SubscriptDefPtr& node) override;
    virtual void visitSubscriptAccess(const SubscriptAccessPtr& node) override;
    virtual void visitForcedValue(const ForcedValuePtr& node) override;
    virtual void visitOptionalChaining(const OptionalChainingPtr& node) override;
    //virtual void visitStruct(const StructDefPtr& node) override;
public:// Condition control flow
    virtual void visitIf(const IfStatementPtr& node) override;
    virtual void visitSwitchCase(const SwitchCasePtr& node) override;
    virtual void visitCase(const CaseStatementPtr& node) override;

    virtual void visitWhileLoop(const WhileLoopPtr& node) override;
    virtual void visitForIn(const ForInLoopPtr& node) override;
    virtual void visitForLoop(const ForLoopPtr& node) override;
    virtual void visitDoLoop(const DoLoopPtr& node) override;
public://Syntax sugar for type
    virtual void visitOptionalType(const OptionalTypePtr& node);
public://properties
    const TypePtr& getCurrentFunction() const;
    const TypePtr& getCurrentType() const;
private:
    void visitAccessor(const CodeBlockPtr& accessor, const ParametersPtr& params, const SymbolPtr& setter);
    TypePtr defineType(const std::shared_ptr<TypeDeclaration>& node);
    void prepareDefaultInitializers(const TypePtr& type);
    //Verify each symbol in the tuple is initialized
    void verifyTuplePattern(const PatternPtr& pattern);

    FunctionSymbolPtr createFunctionSymbol(const FunctionDefPtr& func, const GenericDefinitionPtr& generic);
    TypePtr createFunctionType(const std::vector<ParametersPtr>::const_iterator& begin, const std::vector<ParametersPtr>::const_iterator& end, const TypePtr& retType, const GenericDefinitionPtr& generic);

    /*!
     * Prepare parameters as symbols in given code block
     */
    void prepareParameters(SymbolScope* scope, const ParametersPtr& params);
    void registerSymbol(const SymbolPlaceHolderPtr& symbol, const NodePtr& node);
    GenericDefinitionPtr prepareGenericTypes(const GenericParametersDefPtr& params);

    SymbolPtr visitFunctionCall(const std::vector<SymbolPtr>& func, const ParenthesizedExpressionPtr& args, const PatternPtr& node);
private:
    void checkTupleDefinition(const TuplePtr& tuple, const ExpressionPtr& initializer);
    TypePtr evaluateType(const ExpressionPtr& expr);

    bool hasOptionalChaining(const NodePtr& node);
    bool isParentInOptionalChain(const NodePtr& node);

    /*!
     * Calculates the fit score of arguments on given function
     * @return -1 if the type is not matched
     */
    float calculateFitScore(TypePtr& func, const ParenthesizedExpressionPtr& arguments, bool supressErrors);

    bool checkArgument(const TypePtr& funcType, const Type::Parameter& parameter, const std::pair<std::wstring, ExpressionPtr>& argument, bool variadic, float& score, bool supressErrors, std::map<std::wstring, TypePtr>& genericTypes);
    TypePtr getExpressionType(const ExpressionPtr& expr, const TypePtr& hint, float& score);

    /*!
     * Return a function that matches the given argument
     * This will always returns a matched function, if no functions matched it will throw exception and abort the process
     */
    SymbolPtr getOverloadedFunction(const NodePtr& node, const std::vector<SymbolPtr>& funcs, const ParenthesizedExpressionPtr& arguments);
    /*!
     * Check if the given expression can be converted to given type
     */
    bool canConvertTo(const ExpressionPtr&, const TypePtr& type);

    bool isInteger(const TypePtr& type);
    bool isNumber(const TypePtr& type);
    bool isFloat(const TypePtr& type);


    /*!
     * Verify if the specified type conform to the given protocol
     */
    void verifyProtocolConform(const TypePtr& type);
    void verifyProtocolConform(const TypePtr& type, const TypePtr& protocol);
    void verifyProtocolFunction(const TypePtr& type, const TypePtr& protocol, const FunctionSymbolPtr& expected);


    /*!
     * Need to explode a tuple variable definition into a sequence of single variable definitions
     */
    void explodeValueBindings(const ValueBindingsPtr& node);
    void explodeValueBinding(const ValueBindingsPtr& valueBindings, const std::list<ValueBindingPtr>::iterator& iter);
    MemberAccessPtr makeAccess(SourceInfo* info, NodeFactory* nodeFactory, const std::wstring& tempName, const std::vector<int>& indices);
    void expandTuple(std::vector<TupleExtractionResult>& results, std::vector<int>& indices, const PatternPtr& name, const std::wstring& tempName, const TypePtr& type, PatternAccessibility accessibility);

    /*!
     * Expand given expression to given Optional<T> type by adding implicit Optional<T>.Some calls
     * Return false if the given expression cannot be conform to given optional type
     */
    bool expandOptional(const TypePtr& optionalType, ExpressionPtr& expr);

    /*!
     * Returns the final actual type of Optional in a sequence of Optional type chain.
     * e.g. T?????? will return T
     */
    TypePtr finalTypeOfOptional(const TypePtr& optionalType);

    /*!
     * This will make implicit type conversion like expanding optional
     */
    ExpressionPtr transformExpression(const TypePtr& contextualType, ExpressionPtr expr);

    /*!
     * This will generate a unique temporary name for symbol
     */
    std::wstring generateTempName();
    /*!
     * Convert a AST TypeNode into symboled Type
     */
    TypePtr lookupType(const TypeNodePtr& type, bool supressErrors = false);
    std::wstring toString(const NodePtr& node);
    std::wstring toString(int i);

    /*!
     * Convert expression node to type node
     */
    TypeNodePtr expressionToType(const ExpressionPtr& expr);

    /*!
     * Gets all functions from current scope to top scope with given name, if flagMasks is specified, only functions
     * with given mask will be returned
     */
    std::vector<SymbolPtr> allFunctions(const std::wstring& name, int flagMasks = 0, bool allScopes = true);

    /*!
     * Declaration finished, added it as a member to current type or current type extension.
     */
    void declarationFinished(const std::wstring& name, const SymbolPtr& decl, const NodePtr& node);
private:
    TypePtr lookupTypeImpl(const TypeNodePtr& type, bool supressErrors);

    /*!
     * This implementation will try to find the member from the type, and look up from extension as a fallback.
     */
    SymbolPtr getMemberFromType(const TypePtr& type, const std::wstring& fieldName, bool staticMember);

    /*!
     * This implementation will try to all methods from the type, including defined in parent class or extension
     */
    void getMethodsFromType(const TypePtr& type, const std::wstring& fieldName, bool staticMember, std::vector<SymbolPtr>& result);
private:
    /*!
     * Validate modifiers for declarations.
     */
    void validateDeclarationModifiers(const DeclarationPtr& declaration);
protected:
    //hint for parsing Array/tuple/dictionary literal
    TypePtr t_hint;
    TypePtr currentFunction;
    int numTemporaryNames;
private:
    TypePtr currentType;
    TypePtr currentExtension;
};

SWALLOW_NS_END




#endif//SEMANTIC_ANALYZER_H
