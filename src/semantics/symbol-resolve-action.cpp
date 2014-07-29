#include "symbol-resolve-action.h"
#include "symbol-registry.h"
#include "ast/ast.h"
#include "common/compiler_results.h"
#include "swift_errors.h"
#include "symbol-identifier.h"
#include "scoped-nodes.h"

USE_SWIFT_NS

SymbolResolveAction::SymbolResolveAction(SymbolRegistry* symbolRegistry, CompilerResults* compilerResults)
    :SemanticNodeVisitor(symbolRegistry), compilerResults(compilerResults)
{

}

void SymbolResolveAction::visitAssignment(const AssignmentPtr& node)
{
    //check node's LHS, it must be a tuple or identifier
    PatternPtr pattern = node->getLHS();
    verifyTuplePattern(pattern);
}
void SymbolResolveAction::verifyTuplePattern(const PatternPtr& pattern)
{

    if(pattern->getNodeType() == NodeType::Identifier)
    {
        IdentifierPtr id = std::static_pointer_cast<Identifier>(pattern);
        TypePtr type = symbolRegistry->lookupType(id->getIdentifier());
        if(type)
        {
            //cannot assign expression to type
            compilerResults->add(ErrorLevel::Error, *id->getSourceInfo(), Errors::E_CANNOT_ASSIGN, id->getIdentifier());
            return;
        }
        NodePtr sym = std::dynamic_pointer_cast<Node>(symbolRegistry->lookupSymbol(id->getIdentifier()));
        if(!sym)
        {
            compilerResults->add(ErrorLevel::Error, *id->getSourceInfo(), Errors::E_USE_OF_UNRESOLVED_IDENTIFIER, id->getIdentifier());
        }
    }
    else if(pattern->getNodeType() == NodeType::Tuple)
    {
        TuplePtr tuple = std::static_pointer_cast<Tuple>(pattern);
        for(const PatternPtr& element : tuple->elements)
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

void SymbolResolveAction::registerPattern(const PatternPtr& pattern)
{
    SymbolScope* scope = symbolRegistry->getCurrentScope();
    if(pattern->getNodeType() == NodeType::Identifier)
    {
        IdentifierPtr id = std::static_pointer_cast<Identifier>(pattern);
        SymbolPtr s = symbolRegistry->lookupSymbol(id->getIdentifier());
        if(s)
        {
            //already defined
            compilerResults->add(ErrorLevel::Error, *id->getSourceInfo(), Errors::E_DEFINITION_CONFLICT, id->getIdentifier());
        }
        else
        {
            scope->addSymbol(std::static_pointer_cast<SymbolIdentifier>(id));
        }
    }
    else if(pattern->getNodeType() == NodeType::Tuple)
    {
        TuplePtr tuple = std::static_pointer_cast<Tuple>(pattern);
        for(const PatternPtr& element : tuple->elements)
        {
            registerPattern(element);
        }
    }
}

void SymbolResolveAction::visitVariables(const VariablesPtr& node)
{
    for(const VariablePtr& var : *node)
    {
        PatternPtr name = var->getName();
        registerPattern(name);
    }
    for(const VariablePtr& v : *node)
    {
        PatternPtr name = v->getName();
        ExpressionPtr initializer = v->getInitializer();
        CodeBlockPtr didSet = v->getDidSet();
        CodeBlockPtr willSet = v->getWillSet();
        CodeBlockPtr getter = v->getGetter();
        CodeBlockPtr setter = v->getSetter();
        setFlag(name, true, Identifier::F_INITIALIZING);
        if(initializer)
        {
            initializer->accept(this);
        }
        setFlag(name, true, Identifier::F_INITIALIZED);
        setFlag(name, false, Identifier::F_INITIALIZING);
        if(getter)
        {
            getter->accept(this);
        }
        if(setter)
        {
            std::wstring name = v->getSetterName().empty() ? L"newValue" : v->getSetterName();
            //TODO: replace the symbol to internal value
            ScopedCodeBlockPtr cb = std::static_pointer_cast<ScopedCodeBlock>(setter);
            cb->getScope()->addSymbol(SymbolPtr(new SymbolPlaceHolder(name)));
            cb->accept(this);
        }
        if(willSet)
        {
            std::wstring setter = v->getWillSetSetter().empty() ? L"newValue" : v->getWillSetSetter();
            //TODO: replace the symbol to internal value
            ScopedCodeBlockPtr cb = std::static_pointer_cast<ScopedCodeBlock>(willSet);
            cb->getScope()->addSymbol(SymbolPtr(new SymbolPlaceHolder(setter)));
            cb->accept(this);
        }
        if(didSet)
        {
            std::wstring setter = v->getDidSetSetter().empty() ? L"oldValue" : v->getDidSetSetter();
            //TODO: replace the symbol to internal value
            ScopedCodeBlockPtr cb = std::static_pointer_cast<ScopedCodeBlock>(didSet);
            cb->getScope()->addSymbol(SymbolPtr(new SymbolPlaceHolder(setter)));
            cb->accept(this);
        }

    }

}

void SymbolResolveAction::visitConstants(const ConstantsPtr& node)
{
    for(const ConstantPtr& c : *node)
    {
        registerPattern(c->name);
    }
    //verify initializer
    for(const ConstantPtr& c : *node)
    {
        setFlag(c->name, true, Identifier::F_INITIALIZING);
        c->initializer->accept(this);
        setFlag(c->name, true, Identifier::F_INITIALIZED);
        setFlag(c->name, false, Identifier::F_INITIALIZING);
    }
}

void SymbolResolveAction::visitIdentifier(const IdentifierPtr& id)
{
    SymbolPtr sym = NULL;
    SymbolScope* scope = NULL;
    symbolRegistry->lookupSymbol(id->getIdentifier(), &scope, &sym);
    if(!sym)
    {
        compilerResults->add(ErrorLevel::Error, *id->getSourceInfo(), Errors::E_USE_OF_UNRESOLVED_IDENTIFIER, id->getIdentifier());
        return;
    }
    if(IdentifierPtr identifier = std::dynamic_pointer_cast<Identifier>(sym))
    {
        if((identifier->flags & Identifier::F_INITIALIZING))
        {
            compilerResults->add(ErrorLevel::Error, *id->getSourceInfo(), Errors::E_USE_OF_INITIALIZING_VARIABLE, id->getIdentifier());
        }
        else if((identifier->flags & Identifier::F_INITIALIZED) == 0)
        {
            compilerResults->add(ErrorLevel::Error, *id->getSourceInfo(), Errors::E_USE_OF_UNINITIALIZED_VARIABLE, id->getIdentifier());
        }
        //check if this identifier is accessed inside a class/protocol/extension/struct/enum but defined not in program
        if(dynamic_cast<TypeDeclaration*>(symbolRegistry->getCurrentScope()->getOwner()))
        {
            if(!dynamic_cast<Program*>(scope->getOwner()))
            {
                compilerResults->add(ErrorLevel::Error, *id->getSourceInfo(), Errors::E_USE_OF_FUNCTION_LOCAL_INSIDE_TYPE, id->getIdentifier());
            }
        }


    }
}
void SymbolResolveAction::visitEnumCasePattern(const EnumCasePatternPtr& node)
{
    //TODO: check if the enum case is defined
}

void SymbolResolveAction::setFlag(const PatternPtr& pattern, bool add, int flag)
{
    NodeType::T t = pattern->getNodeType();
    if(t == NodeType::Identifier)
    {
        IdentifierPtr id = std::static_pointer_cast<Identifier>(pattern);
        if(add)
            id->flags |= flag;
        else
            id->flags &= ~flag;
    }
    else if(t == NodeType::Tuple)
    {
        TuplePtr tuple = std::static_pointer_cast<Tuple>(pattern);
        for(const PatternPtr& element : tuple->elements)
        {
            setFlag(element, add, flag);
        }
    }

}

void SymbolResolveAction::defineType(const std::shared_ptr<TypeDeclaration>& node, Type::Category category)
{
    TypeIdentifierPtr id = node->getIdentifier();
    SymbolScope* scope = NULL;
    TypePtr type;
    //it's inside the type's scope, so need to access parent scope;
    SymbolScope* currentScope = symbolRegistry->getCurrentScope()->getParentScope();

    //check if this type is already defined
    symbolRegistry->lookupType(id->getName(), &scope, &type);
    if(type && scope == currentScope)
    {
        //invalid redeclaration of type T
        compilerResults->add(ErrorLevel::Error, *node->getSourceInfo(), Errors::E_INVALID_REDECLARATION, id->getName());
        return;
    }
    //check for parent class or protocol
    for(const TypeIdentifierPtr& parent : node->parents)
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
    type = Type::newType(id->getName(), category);
    currentScope->addSymbol(type);
}


void SymbolResolveAction::visitClass(const ClassDefPtr& node)
{
    defineType(node, Type::Class);
    NodeVisitor::visitClass(node);
}
void SymbolResolveAction::visitStruct(const StructDefPtr& node)
{
    defineType(node, Type::Struct);
    NodeVisitor::visitStruct(node);
}
void SymbolResolveAction::visitEnum(const EnumDefPtr& node)
{
    defineType(node, Type::Enum);
    NodeVisitor::visitEnum(node);
}
void SymbolResolveAction::visitProtocol(const ProtocolDefPtr& node)
{
    defineType(node, Type::Protocol);
    NodeVisitor::visitProtocol(node);
}
void SymbolResolveAction::visitExtension(const ExtensionDefPtr& node)
{
    NodeVisitor::visitExtension(node);

}
void SymbolResolveAction::visitFunction(const FunctionDefPtr& node)
{
    symbolRegistry->getCurrentScope()->addSymbol(std::static_pointer_cast<SymboledFunction>(node));
    NodeVisitor::visitFunction(node);
}
