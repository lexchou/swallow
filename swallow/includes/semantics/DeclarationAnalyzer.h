/* DeclarationAnalyzer.h --
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
#ifndef DECLARATION_ANALYZER_H
#define DECLARATION_ANALYZER_H
#include "SemanticPass.h"
#include "Type.h"
#include <list>
#include <ast/SubscriptDef.h>

SWALLOW_NS_BEGIN

class TypeDeclaration;
class Expression;
class Pattern;
class NodeFactory;
typedef std::shared_ptr<class Identifier> IdentifierPtr;

class SemanticAnalyzer;
struct SemanticContext;
/*!
 * This analyzer will make a lazy declaration on types and functions
 */
class SWALLOW_EXPORT DeclarationAnalyzer : public SemanticPass
{
    enum
    {
        D_VARIABLE = 0,
        D_PROPERTY,
        D_FUNCTION,
        D_METHOD,
        D_SUBSCRIPT,
        D_INITIALIZER,
        D_CLASS,
        D_ENUM,

        C_TYPE = 0,
        C_SUPERCLASS,
        C_PARAMETER,
        C_RESULT,
        C_INDEX,
        C_ELEMENT_TYPE,
        C_RAW_TYPE
    };
public:
    DeclarationAnalyzer(SemanticAnalyzer* semanticAnalyzer, SemanticContext* ctx);
public:
    virtual void visitTypeAlias(const TypeAliasPtr& node) override;
    virtual void visitClass(const ClassDefPtr& node) override;
    virtual void visitStruct(const StructDefPtr& node) override;
    virtual void visitEnum(const EnumDefPtr& node) override;
    virtual void visitProtocol(const ProtocolDefPtr& node) override;
    virtual void visitExtension(const ExtensionDefPtr& node) override;
    virtual void visitFunction(const FunctionDefPtr& node) override;
    void visitFunctionDeclaration(const FunctionDefPtr& node);
    virtual void visitParameter(const ParameterNodePtr& node) override;
    virtual void visitParameters(const ParametersNodePtr& node) override;
    virtual void visitClosure(const ClosurePtr& node) override;
    virtual void visitSubscript(const SubscriptDefPtr &node) override;
    virtual void visitDeinit(const DeinitializerDefPtr& node) override;
    virtual void visitInit(const InitializerDefPtr& node) override;
    virtual void visitCodeBlock(const CodeBlockPtr &node) override;
    void visitAccessor(const CodeBlockPtr& accessor, const ParametersNodePtr& params, const SymbolPtr& setter, int modifiers);
    virtual void visitComputedProperty(const ComputedPropertyPtr& node) override;
    virtual void visitValueBinding(const ValueBindingPtr& node) override;
    virtual void visitValueBindings(const ValueBindingsPtr& node) override;

    /*!
     * Only visits the implementation of members of given type definition
     */
    void visitImplementation(const TypeDeclarationPtr& node);
    /*!
     * Only visits the declaration of members of given type definition
     */
    void visitDeclaration(const TypeDeclarationPtr& node);
public:

    /*!
     * Verify if the specified type conform to the given protocol
     */
    void verifyProtocolConform(const TypePtr& type);

public://properties
    const TypePtr& getCurrentFunction() const;
    const TypePtr& getCurrentType() const;
    SemanticAnalyzer* getSemanticAnalyzer() {return semanticAnalyzer;}
private:
    TypePtr defineType(const std::shared_ptr<TypeDeclaration>& node);
    void prepareDefaultInitializers(const TypePtr& type);
    FunctionSymbolPtr createFunctionSymbol(const FunctionDefPtr& func, const GenericDefinitionPtr& generic);
    TypePtr createFunctionType(const std::vector<ParametersNodePtr>::const_iterator& begin, const std::vector<ParametersNodePtr>::const_iterator& end, const TypePtr& retType, const GenericDefinitionPtr& generic);

    /*!
     * Prepare parameters as symbols in given code block
     */
    void prepareParameters(SymbolScope* scope, const ParametersNodePtr& params);

    void verifyProtocolConform(const TypePtr& type, const TypePtr& protocol);
    void verifyProtocolFunction(const TypePtr& type, const TypePtr& protocol, const FunctionSymbolPtr& expected);

    GenericDefinitionPtr prepareGenericTypes(const GenericParametersDefPtr& params);

    /*!
     * Validate modifiers for declarations.
     */
    void validateDeclarationModifiers(const DeclarationPtr& declaration);
    /*!
     * Convert a AST TypeNode into symboled Type
     */
    TypePtr lookupType(const TypeNodePtr& type, bool supressErrors = false);
    /*!
     * Declaration finished, added it as a member to current type or current type extension.
     */
    void declarationFinished(const std::wstring& name, const SymbolPtr& decl, const NodePtr& node);

    TypePtr defineType(const TypeDeclarationPtr& decl, Type::Category category);
private:
    void checkForFunctionOverriding(const std::wstring& name, const FunctionSymbolPtr& decl, const DeclarationPtr& node);
    void checkForPropertyOverriding(const std::wstring& name, const ComputedPropertySymbolPtr& decl, const ComputedPropertyPtr& node);
    void checkForSubscriptOverride(const Subscript& subscript, const SubscriptDefPtr& node);
    /*!
     * Parse access level from a set of declaration modifiers
     */
    AccessLevel parseAccessLevel(int modifiers);
    /*!
     * Register the symbol to current scope
     */
    void registerSymbol(const SymbolPlaceHolderPtr& symbol, const NodePtr& node);
    /*!
     * Check if given initializer expression can be assigned to given tuple of variables
     */
    void checkTupleDefinition(const TuplePtr& tuple, const ExpressionPtr& initializer);
    /*!
     * Need to explode a tuple variable definition into a sequence of single variable definitions
     */
    void explodeValueBindings(const ValueBindingsPtr& node);

    /*!
     * Verify access level
     */
    void verifyAccessLevel(const DeclarationPtr& node, const TypePtr& type, int declaration, int component);
protected:
    SemanticAnalyzer* semanticAnalyzer;

    SemanticContext* ctx;
};

SWALLOW_NS_END




#endif//DECLARATION_ANALYZER_H
