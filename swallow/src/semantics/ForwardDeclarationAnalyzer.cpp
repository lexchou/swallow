#include "semantics/ForwardDeclarationAnalyzer.h"
#include "semantics/SymbolRegistry.h"
#include "semantics/SymbolScope.h"
#include "semantics/Type.h"
#include "semantics/ScopeGuard.h"
#include "semantics/ScopedNodes.h"
#include "common/Errors.h"
#include "ast/ast.h"

USE_SWALLOW_NS
using namespace std;

ForwardDeclarationAnalyzer::ForwardDeclarationAnalyzer(SymbolRegistry* symbolRegistry, CompilerResults* results)
    :SemanticPass(symbolRegistry, results)
{
}
void ForwardDeclarationAnalyzer::visitTypeAlias(const TypeAliasPtr& node)
{
    TypePtr type = forwardDeclare(node, node->getName(), Type::Alias);
    SemanticPass::visitTypeAlias(node);
}
void ForwardDeclarationAnalyzer::visitClass(const ClassDefPtr& node)
{
    TypePtr type = forwardDeclare(node, node->getIdentifier()->getName(), Type::Class);
    ScopeGuard scope(symbolRegistry, type->getScope());
    SemanticPass::visitClass(node);
}
void ForwardDeclarationAnalyzer::visitStruct(const StructDefPtr& node)
{
    TypePtr type = forwardDeclare(node, node->getIdentifier()->getName(), Type::Struct);
    ScopeGuard scope(symbolRegistry, type->getScope());
    SemanticPass::visitStruct(node);
}
void ForwardDeclarationAnalyzer::visitEnum(const EnumDefPtr& node)
{
    TypePtr type = forwardDeclare(node, node->getIdentifier()->getName(), Type::Enum);
    ScopeGuard scope(symbolRegistry, type->getScope());
    SemanticPass::visitEnum(node);
}
void ForwardDeclarationAnalyzer::visitProtocol(const ProtocolDefPtr& node)
{
    TypePtr type = forwardDeclare(node, node->getIdentifier()->getName(), Type::Protocol);
    ScopeGuard scope(symbolRegistry, type->getScope());
    SemanticPass::visitProtocol(node);
}
void ForwardDeclarationAnalyzer::visitExtension(const ExtensionDefPtr& node)
{
    //TODO enter type's scope
    SemanticPass::visitExtension(node);
}


TypePtr ForwardDeclarationAnalyzer::forwardDeclare(const NodePtr& node, const std::wstring& name, Type::Category category)
{
    SymbolScope* scope = symbolRegistry->getCurrentScope();
    //check if this type is already defined
    if(scope->isSymbolDefined(name))
    {
        //invalid redeclaration of type T
        error(node, Errors::E_INVALID_REDECLARATION_1, name);
        return nullptr;
    }
    TypePtr type = Type::newType(name, category);
    scope->addForwardDeclaration(type);
    return type;
}
