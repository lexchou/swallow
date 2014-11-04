/* SemanticAnalyzer.h --
 *
 * Copyright (c) 2014, Lex Chou <lex at chou dot com>
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
#include "ast/NodeVisitor.h"
#include "Type.h"
#include <list>

SWALLOW_NS_BEGIN

class SymbolRegistry;
class CompilerResults;
class TypeDeclaration;
class Expression;
class Pattern;
class NodeFactory;
class SemanticAnalyzer : public NodeVisitor
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
    virtual void visitParameters(const ParametersPtr& node) override;
public:

    virtual void visitFunctionCall(const FunctionCallPtr& node) override;
    virtual void visitOperator(const OperatorDefPtr& node) override;
    virtual void visitValueBinding(const ValueBindingPtr& node) override;
    virtual void visitString(const StringLiteralPtr& node) override;
    virtual void visitInteger(const IntegerLiteralPtr& node) override;
    virtual void visitFloat(const FloatLiteralPtr& node) override;
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
    //virtual void visitStruct(const StructDefPtr& node) override;
public:
    /*
    virtual void visitValueBindings(const ValueBindingsPtr& node) override;
    virtual void visitValueBinding(const ValueBindingPtr& node) override;
    virtual void visitClass(const ClassDefPtr& node) override;
    virtual void visitStruct(const StructDefPtr& node) override;
    virtual void visitEnum(const EnumDefPtr& node) override;
    virtual void visitExtension(const ExtensionDefPtr& node) override;
    virtual void visitProtocol(const ProtocolDefPtr& node) override;
    */
    SymbolRegistry* getSymbolRegistry() { return symbolRegistry;}
private:
    void visitAccessor(const CodeBlockPtr& accessor, const ParametersPtr& params, const SymbolPtr& setter);
    TypePtr defineType(const std::shared_ptr<TypeDeclaration>& node, Type::Category category);
    //Verify each symbol in the tuple is initialized
    void verifyTuplePattern(const PatternPtr& pattern);

    FunctionSymbolPtr createFunctionSymbol(const FunctionDefPtr& func, const GenericDefinitionPtr& generic);
    TypePtr createFunctionType(const std::vector<ParametersPtr>::const_iterator& begin, const std::vector<ParametersPtr>::const_iterator& end, const TypePtr& retType, const GenericDefinitionPtr& generic);

    /**
    * Prepare parameters as symbols in given code block
    */
    void prepareParameters(SymbolScope* scope, const ParametersPtr& params);
    void registerSymbol(const SymbolPlaceHolderPtr& symbol);
    GenericDefinitionPtr prepareGenericTypes(const GenericParametersDefPtr& params);

    void visitFunctionCall(const IdentifierPtr& name, const FunctionCallPtr& node);
    void visitFunctionCall(const SymbolPtr& func, const FunctionCallPtr& node);
private:
    void checkTupleDefinition(const TuplePtr& tuple, const ExpressionPtr& initializer);
    TypePtr evaluateType(const ExpressionPtr& expr);

    /**
    * Calculates the fit score of arguments on given function
    * @return -1 if the type is not matched
    */
    float calculateFitScore(const TypePtr& func, const ParenthesizedExpressionPtr& arguments, bool supressErrors);

    bool checkArgument(const TypePtr& funcType, const Type::Parameter& parameter, const std::pair<std::wstring, ExpressionPtr>& argument, bool variadic, float& score, bool supressErrors);
    TypePtr getExpressionType(const ExpressionPtr& expr, const TypePtr& hint, float& score);

    /**
    * Return a function that matches the given argument
    * This will always returns a matched function, if no functions matched it will throw exception and abort the process
    */
    FunctionSymbolPtr getOverloadedFunction(const NodePtr& node, const FunctionOverloadedSymbolPtr& funcs, const ParenthesizedExpressionPtr& arguments);
    /**
    * Check if the given expression can be converted to given type
    */
    bool canConvertTo(const ExpressionPtr&, const TypePtr& type);

    bool isInteger(const TypePtr& type);
    bool isNumber(const TypePtr& type);
    bool isFloat(const TypePtr& type);


    /**
    * Verify if the specified type conform to the given protocol
    */
    void verifyProtocolConform(const TypePtr& type);
    void verifyProtocolConform(const TypePtr& type, const TypePtr& protocol);
    void verifyProtocolFunction(const TypePtr& type, const TypePtr& protocol, const FunctionSymbolPtr& expected);


    /**
     * Need to explode a tuple variable definition into a sequence of single variable definitions
     */
    void explodeValueBindings(const ValueBindingsPtr& node);
    void explodeValueBinding(const ValueBindingsPtr& valueBindings, const std::list<ValueBindingPtr>::iterator& iter);
    MemberAccessPtr makeAccess(SourceInfo* info, NodeFactory* nodeFactory, const std::wstring& tempName, const std::vector<int>& indices);
    void expandTuple(std::vector<std::tuple<IdentifierPtr, TypePtr, ExpressionPtr> >& results, std::vector<int>& indices, const PatternPtr& name, const std::wstring& tempName, const TypePtr& type, bool isReadonly);

protected:
    /**
    * Abort the visitor
    */
    void abort();

    /**
    * Outputs an compiler error
    */
    void error(const NodePtr& node, int code);
    void error(const NodePtr& node, int code, const std::vector<std::wstring>& items);
    void error(const NodePtr& node, int code, const std::wstring& item);
    void error(const NodePtr& node, int code, const std::wstring& item1, const std::wstring& item2);
    void error(const NodePtr& node, int code, const std::wstring& item1, const std::wstring& item2, const std::wstring& item3);
    void error(const NodePtr& node, int code, const std::wstring& item1, const std::wstring& item2, const std::wstring& item3, const std::wstring& item4);


    /**
    * Outputs an compiler error
    */
    void warning(const NodePtr& node, int code, const std::wstring& item = std::wstring());


    /**
    * Convert a AST TypeNode into symboled Type
    */
    TypePtr lookupType(const TypeNodePtr& type);
    std::wstring toString(const NodePtr& node);
    std::wstring toString(int i);
private:
    TypePtr lookupTypeImpl(const TypeNodePtr& type);
protected:
    SymbolRegistry* symbolRegistry;
    CompilerResults* compilerResults;
private:
    //hint for parsing Array/tuple/dictionary literal
    TypePtr t_hint;
private:
    TypePtr currentType;
};



template<class T>
struct StackedValueGuard
{
    StackedValueGuard(T & value)
            :ref(value), value(value)
    {

    }
    void set(const T& val)
    {
        ref = val;
    }

    ~StackedValueGuard()
    {
        ref = value;
    }

    T& ref;
    T value;

};

SWALLOW_NS_END




#endif//SEMANTIC_ANALYZER_H
