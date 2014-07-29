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
    virtual void visitAssignment(const AssignmentPtr& node);
    virtual void visitVariables(const VariablesPtr& node);
    virtual void visitConstants(const ConstantsPtr& node);
    virtual void visitClass(const ClassDefPtr& node);
    virtual void visitStruct(const StructDefPtr& node);
    virtual void visitEnum(const EnumDefPtr& node);
    virtual void visitProtocol(const ProtocolDefPtr& node);
    virtual void visitExtension(const ExtensionDefPtr& node);
    virtual void visitFunction(const FunctionDefPtr& node);
    virtual void visitIdentifier(const IdentifierPtr& id);
    virtual void visitEnumCasePattern(const EnumCasePatternPtr& node);
private:
    void defineType(const std::shared_ptr<TypeDeclaration>& node, Type::Category category);
    //Register all symbols in the pattern
    void registerPattern(const PatternPtr& pattern);
    //Verify each symbol in the tuple is initialized
    void verifyTuplePattern(const PatternPtr& pattern);

    //set or reset flag in all identifiers in given pattern
    void setFlag(const PatternPtr& pattern, bool add, int flag);
private:
    CompilerResults* compilerResults;
};

SWIFT_NS_END


#endif//SYMBOL_RESOLVE_ACTION_H