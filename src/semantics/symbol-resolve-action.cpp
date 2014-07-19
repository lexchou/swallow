#include "symbol-resolve-action.h"
#include "symbol-registry.h"
#include "ast/ast.h"
#include "common/compiler_results.h"
#include "swift_errors.h"

USE_SWIFT_NS

SymbolResolveAction::SymbolResolveAction(SymbolRegistry* symbolRegistry, CompilerResults* compilerResults)
    :symbolRegistry(symbolRegistry), compilerResults(compilerResults)
{

}

void SymbolResolveAction::visitAssignment(Assignment* node)
{
    //check node's LHS, it must be a tuple or identifier
    Pattern* pattern = node->getLHS();
    verifyTuplePattern(pattern);
}
void SymbolResolveAction::verifyTuplePattern(Pattern* pattern)
{

    if(pattern->getNodeType() == NodeType::Identifier)
    {
        Identifier* id = static_cast<Identifier*>(pattern);
        TypePtr type = symbolRegistry->lookupType(id->getIdentifier());
        if(type)
        {
            //cannot assign expression to type
            compilerResults->add(ErrorLevel::Error, *id->getSourceInfo(), Errors::E_CANNOT_ASSIGN, id->getIdentifier());
            return;
        }
        Node* sym = symbolRegistry->lookupSymbol(id->getIdentifier());
        if(!sym)
        {
            compilerResults->add(ErrorLevel::Error, *id->getSourceInfo(), Errors::E_USE_OF_UNRESOLVED_IDENTIFIER, id->getIdentifier());
        }
    }
    else if(pattern->getNodeType() == NodeType::Tuple)
    {
        Tuple* tuple = static_cast<Tuple*>(pattern);
        for(Pattern* element : tuple->elements)
        {
            verifyTuplePattern(element);
        }
    }
    else
    {
        compilerResults->add(ErrorLevel::Error, *pattern->getSourceInfo(), Errors::E_CANNOT_ASSIGN);
        return;
    }

}

void SymbolResolveAction::visitVariables(Variables* node)
{
    for(Variable* var : node->variables)
    {
        Pattern* name = var->getName();
        registerPattern(name);
    }
}
void SymbolResolveAction::registerPattern(Pattern* pattern)
{
    if(pattern->getNodeType() == NodeType::Identifier)
    {
        Identifier* id = static_cast<Identifier*>(pattern);
        symbolRegistry->getCurrentScope()->addSymbol(id->getIdentifier(), id);
    }
    else if(pattern->getNodeType() == NodeType::Tuple)
    {
        Tuple* tuple = static_cast<Tuple*>(pattern);
        for(Pattern* element : tuple->elements)
        {
            registerPattern(element);
        }
    }
}
void SymbolResolveAction::visitConstants(Constant* node)
{
    for(const std::pair<Pattern*, Expression*>& c : node->pairs)
    {
        registerPattern(c.first);
    }
}
void SymbolResolveAction::visitClass(ClassDef* node)
{
    defineType(node, Type::Class);
}

void SymbolResolveAction::defineType(TypeDeclaration* node, Type::Category category)
{
    TypeIdentifier* id = node->getIdentifier();
    SymbolScope* scope = NULL;
    TypePtr type;
    //check if this type is already defined
    symbolRegistry->lookupType(id->getName(), &scope, &type);
    if(type && scope == symbolRegistry->getCurrentScope())
    {
        //invalid redeclaration of type T
        compilerResults->add(ErrorLevel::Error, *node->getSourceInfo(), Errors::E_INVALID_REDECLARATION, id->getName());
        return;
    }
    //check for parent class or protocol
    for(TypeIdentifier* parent : node->parents)
    {
        //check the parent must be a defined type
        type = symbolRegistry->lookupType(parent->getName());
        if(!type)
        {
            compilerResults->add(ErrorLevel::Error, *node->getSourceInfo(), Errors::E_USE_OF_UNDECLARED_TYPE, parent->getName());
            return;
        }
    }
    //register this type
    type.reset(Type::newType(id->getName(), category));
    symbolRegistry->getCurrentScope()->addType(type);
}

void SymbolResolveAction::visitStruct(StructDef* node)
{
    defineType(node, Type::Struct);
}
void SymbolResolveAction::visitEnum(EnumDef* node)
{
    defineType(node, Type::Enum);
}
void SymbolResolveAction::visitProtocol(ProtocolDef* node)
{
    defineType(node, Type::Protocol);
}
void SymbolResolveAction::visitExtension(ExtensionDef* node)
{

}
void SymbolResolveAction::visitFunction(FunctionDef* node)
{
    symbolRegistry->getCurrentScope()->addSymbol(node->getName(), node);
}
