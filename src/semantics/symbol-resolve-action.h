#ifndef SYMBOL_RESOLVE_ACTION_H
#define SYMBOL_RESOLVE_ACTION_H
#include "swift_conf.h"
#include "ast/node-visitor.h"
#include "semantics/type.h"
SWIFT_NS_BEGIN

class SymbolRegistry;
class CompilerResults;
class TypeDeclaration;
class Pattern;
class SymbolResolveAction : public NodeVisitor
{
public:
    SymbolResolveAction(SymbolRegistry* symbolRegistry, CompilerResults* compilerResults);
public:
    virtual void visitAssignment(Assignment* node);
    virtual void visitVariables(Variables* node);
    virtual void visitConstants(Constant* node);
    virtual void visitClass(ClassDef* node);
    virtual void visitStruct(StructDef* node);
    virtual void visitEnum(EnumDef* node);
    virtual void visitProtocol(ProtocolDef* node);
    virtual void visitExtension(ExtensionDef* node);
    virtual void visitFunction(FunctionDef* node);
private:
    void defineType(TypeDeclaration* node, Type::Category category);
    void registerPattern(Pattern* pattern);
    void verifyTuplePattern(Pattern* pattern);
private:
    SymbolRegistry* symbolRegistry;
    CompilerResults* compilerResults;
};

SWIFT_NS_END


#endif//SYMBOL_RESOLVE_ACTION_H