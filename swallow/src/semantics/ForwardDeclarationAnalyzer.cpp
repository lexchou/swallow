#include "semantics/ForwardDeclarationAnalyzer.h"
#include "semantics/SymbolRegistry.h"
#include "semantics/SymbolScope.h"
#include "semantics/Type.h"
#include "semantics/ScopeGuard.h"
#include "semantics/ScopedNodes.h"
#include "ast/ast.h"

USE_SWALLOW_NS
using namespace std;

ForwardDeclarationAnalyzer::ForwardDeclarationAnalyzer(SemanticPass* semanticPass)
:SemanticPass(semanticPass)
{
}
void ForwardDeclarationAnalyzer::visitTypeAlias(const TypeAliasPtr& node)
{
    forwardDeclare(node->getName(), Type::Alias);
    SemanticPass::visitTypeAlias(node);
}
void ForwardDeclarationAnalyzer::visitClass(const ClassDefPtr& node)
{
    forwardDeclare(node->getIdentifier()->getName(), Type::Class);
    ScopeGuard scope(static_cast<ScopedClass*>(node.get()), this);
    SemanticPass::visitClass(node);
}
void ForwardDeclarationAnalyzer::visitStruct(const StructDefPtr& node)
{
    forwardDeclare(node->getIdentifier()->getName(), Type::Struct);
    ScopeGuard scope(static_cast<ScopedStruct*>(node.get()), this);
    SemanticPass::visitStruct(node);
}
void ForwardDeclarationAnalyzer::visitEnum(const EnumDefPtr& node)
{
    forwardDeclare(node->getIdentifier()->getName(), Type::Enum);
    ScopeGuard scope(static_cast<ScopedEnum*>(node.get()), this);
    SemanticPass::visitEnum(node);
}
void ForwardDeclarationAnalyzer::visitProtocol(const ProtocolDefPtr& node)
{
    forwardDeclare(node->getIdentifier()->getName(), Type::Protocol);
    ScopeGuard scope(static_cast<ScopedProtocol*>(node.get()), this);
    SemanticPass::visitProtocol(node);
}
void ForwardDeclarationAnalyzer::visitExtension(const ExtensionDefPtr& node)
{
    //TODO enter type's scope
    SemanticPass::visitExtension(node);
}


void ForwardDeclarationAnalyzer::forwardDeclare(const std::wstring& name, Type::Category category)
{
    TypePtr type = Type::newType(name, category);
    SymbolScope* scope = symbolRegistry->getCurrentScope();
    scope->addForwardDeclaration(type);
}
