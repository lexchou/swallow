#ifndef SYMBOL_RESOLVE_ACTION_H
#define SYMBOL_RESOLVE_ACTION_H
#include "swift_conf.h"
#include "ast/node-visitor.h"
#include "semantics/type.h"
#include "semantic-node-visitor.h"
SWIFT_NS_BEGIN

class SymbolRegistry;
class CompilerResults;
class TypeDeclaration;
class Expression;
class Pattern;
class SymbolResolveAction : public SemanticNodeVisitor
{
public:
    SymbolResolveAction(SymbolRegistry* symbolRegistry, CompilerResults* compilerResults);
public:
    virtual void visitAssignment(const AssignmentPtr& node) override;
    virtual void visitVariables(const VariablesPtr& node) override;
    virtual void visitConstants(const ConstantsPtr& node) override;
    virtual void visitClass(const ClassDefPtr& node) override;
    virtual void visitStruct(const StructDefPtr& node) override;
    virtual void visitEnum(const EnumDefPtr& node) override;
    virtual void visitProtocol(const ProtocolDefPtr& node) override;
    virtual void visitExtension(const ExtensionDefPtr& node) override;
    virtual void visitFunction(const FunctionDefPtr& node) override;
    virtual void visitIdentifier(const IdentifierPtr& id) override;
    virtual void visitEnumCasePattern(const EnumCasePatternPtr& node) override;
    virtual void visitParameter(const ParameterPtr& node) override;
    virtual void visitDeinit(const DeinitializerDefPtr& node) override;
    virtual void visitInit(const InitializerDefPtr& node) override;
    virtual void visitParameters(const ParametersPtr& node) override;

private:
    void defineType(const std::shared_ptr<TypeDeclaration>& node, Type::Category category);
    //Register all symbols in the pattern
    void registerPattern(const PatternPtr& pattern);
    //Verify each symbol in the tuple is initialized
    void verifyTuplePattern(const PatternPtr& pattern);

    //set or reset flag in all identifiers in given pattern
    void setFlag(const PatternPtr& pattern, bool add, int flag);
    FunctionSymbolPtr createFunctionSymbol(const FunctionDefPtr& func);
    TypePtr createFunctionType(const std::vector<ParametersPtr>::const_iterator& begin, const std::vector<ParametersPtr>::const_iterator& end, const TypePtr& retType);

    /**
     * Prepare parameters as symbols in given code block
     */
    void prepareParameters(const CodeBlockPtr& codeBlock, const ParametersPtr& params);
};

SWIFT_NS_END


#endif//SYMBOL_RESOLVE_ACTION_H