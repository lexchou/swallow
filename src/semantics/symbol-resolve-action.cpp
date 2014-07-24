#include "symbol-resolve-action.h"
#include "symbol-registry.h"
#include "ast/ast.h"
#include "common/compiler_results.h"
#include "swift_errors.h"

USE_SWIFT_NS

SymbolResolveAction::SymbolResolveAction(SymbolRegistry* symbolRegistry, CompilerResults* compilerResults)
    :SemanticNodeVisitor(symbolRegistry), compilerResults(compilerResults)
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

void SymbolResolveAction::registerPattern(Pattern* pattern)
{
    SymbolScope* scope = symbolRegistry->getCurrentScope();
    if(pattern->getNodeType() == NodeType::Identifier)
    {
        Identifier* id = static_cast<Identifier*>(pattern);
        Node* s = symbolRegistry->lookupSymbol(id->getIdentifier());
        if(s)
        {
            //already defined
            compilerResults->add(ErrorLevel::Error, *id->getSourceInfo(), Errors::E_DEFINITION_CONFLICT, id->getIdentifier());
        }
        else
        {
            scope->addSymbol(id->getIdentifier(), id);
        }
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

void SymbolResolveAction::visitVariables(Variables* node)
{
    for(Variable* var : node->variables)
    {
        Pattern* name = var->getName();
        registerPattern(name);
    }
}

void SymbolResolveAction::visitConstants(Constants* node)
{
    for(Constant* c : *node)
    {
        registerPattern(c->name);
    }
    //verify initializer
    for(Constant* c : *node)
    {
        setFlag(c->name, true, Identifier::F_INITIALIZING);
        c->initializer->accept(this);
        setFlag(c->name, true, Identifier::F_INITIALIZED);
        setFlag(c->name, false, Identifier::F_INITIALIZING);
    }
}

void SymbolResolveAction::visitIdentifier(Identifier* id)
{
    Node* sym = symbolRegistry->lookupSymbol(id->getIdentifier());
    if(!sym)
    {
        compilerResults->add(ErrorLevel::Error, *id->getSourceInfo(), Errors::E_USE_OF_UNRESOLVED_IDENTIFIER, id->getIdentifier());
        return;
    }
    if(Identifier* identifier = dynamic_cast<Identifier*>(sym))
    {
        if((identifier->flags & Identifier::F_INITIALIZING))
        {
            compilerResults->add(ErrorLevel::Error, *id->getSourceInfo(), Errors::E_USE_OF_INITIALIZING_VARIABLE, id->getIdentifier());
        }
        else if((identifier->flags & Identifier::F_INITIALIZED) == 0)
        {
            compilerResults->add(ErrorLevel::Error, *id->getSourceInfo(), Errors::E_USE_OF_UNINITIALIZED_VARIABLE, id->getIdentifier());
        }
    }
}
void SymbolResolveAction::visitEnumCasePattern(EnumCasePattern* node)
{
    //TODO: check if the enum case is defined
}

void SymbolResolveAction::setFlag(Pattern* pattern, bool add, int flag)
{
    NodeType::T t = pattern->getNodeType();
    if(t == NodeType::Identifier)
    {
        Identifier* id = static_cast<Identifier*>(pattern);
        if(add)
            id->flags |= flag;
        else
            id->flags &= ~flag;
    }
    else if(t == NodeType::Tuple)
    {
        Tuple* tuple = static_cast<Tuple*>(pattern);
        for(Pattern* element : tuple->elements)
        {
            setFlag(element, add, flag);
        }
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
