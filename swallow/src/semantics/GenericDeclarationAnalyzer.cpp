#include "semantics/GenericDeclarationAnalyzer.h"
#include "semantics/SymbolRegistry.h"
#include "semantics/SymbolScope.h"
#include "semantics/Type.h"
#include "semantics/ScopeGuard.h"
#include "semantics/ScopedNodes.h"
#include "semantics/DeclarationAnalyzer.h"
#include "semantics/GenericDefinition.h"
#include "semantics/TypeBuilder.h"
#include "common/Errors.h"
#include "ast/ast.h"
#include <cassert>

USE_SWALLOW_NS
using namespace std;

GenericDeclarationAnalyzer::GenericDeclarationAnalyzer(SymbolRegistry* symbolRegistry, CompilerResults* results, DeclarationAnalyzer* declarationAnalyzer)
    :SemanticPass(symbolRegistry, results)
{
    this->declarationAnalyzer = declarationAnalyzer;
}
void GenericDeclarationAnalyzer::visitClass(const ClassDefPtr& node)
{
    TypePtr type = defineGeneric(node);
    ScopeGuard scope(symbolRegistry, type->getScope());
    SemanticPass::visitClass(node);
}
void GenericDeclarationAnalyzer::visitStruct(const StructDefPtr& node)
{
    TypePtr type = defineGeneric(node);
    ScopeGuard scope(symbolRegistry, type->getScope());
    SemanticPass::visitStruct(node);
}
void GenericDeclarationAnalyzer::visitEnum(const EnumDefPtr& node)
{
    TypePtr type = defineGeneric(node);
    ScopeGuard scope(symbolRegistry, type->getScope());
    SemanticPass::visitEnum(node);
}
void GenericDeclarationAnalyzer::visitProtocol(const ProtocolDefPtr& node)
{
    TypePtr type = defineGeneric(node);
    ScopeGuard scope(symbolRegistry, type->getScope());
    SemanticPass::visitProtocol(node);
}


TypePtr GenericDeclarationAnalyzer::defineGeneric(const TypeDeclarationPtr& node)
{
    GenericDefinitionPtr generic;
    TypeBuilderPtr type = static_pointer_cast<TypeBuilder>(node->getType());
    assert(type != nullptr);
    if(node->getGenericParametersDef() && !type->getGenericDefinition())
    {
        GenericParametersDefPtr genericParams = node->getGenericParametersDef();
        generic = declarationAnalyzer->prepareGenericTypes(genericParams);
        generic->registerTo(type->getScope());
        type->setGenericDefinition(generic);
    }
    return type;
}
