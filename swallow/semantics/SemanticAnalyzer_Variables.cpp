#include "SemanticAnalyzer.h"
#include "ast/ast.h"
#include "Symbol.h"
#include "SymbolScope.h"
#include "SymbolRegistry.h"
#include "swift_errors.h"
#include "ScopedNodes.h"
#include "TypeBuilder.h"
#include "common/CompilerResults.h"
#include <cassert>
#include "GlobalScope.h"

USE_SWIFT_NS
using namespace std;

void SemanticAnalyzer::visitAssignment(const AssignmentPtr& node)
{
    //check node's LHS, it must be a tuple or identifier
    PatternPtr pattern = node->getLHS();
    verifyTuplePattern(pattern);
}
void SemanticAnalyzer::verifyTuplePattern(const PatternPtr& pattern)
{

    if(pattern->getNodeType() == NodeType::Identifier)
    {
        IdentifierPtr id = std::static_pointer_cast<Identifier>(pattern);
        TypePtr type = symbolRegistry->lookupType(id->getIdentifier());
        if(type)
        {
            //cannot assign expression to type
            error(id, Errors::E_CANNOT_ASSIGN, id->getIdentifier());
            return;
        }
        SymbolPtr sym = symbolRegistry->lookupSymbol(id->getIdentifier());
        if(!sym)
        {
            error(id, Errors::E_USE_OF_UNRESOLVED_IDENTIFIER_1, id->getIdentifier());
        }
    }
    else if(pattern->getNodeType() == NodeType::Tuple)
    {
        TuplePtr tuple = std::static_pointer_cast<Tuple>(pattern);
        for(const Tuple::Element& element : tuple->elements)
        {
            verifyTuplePattern(element.element);
        }
    }
    else
    {
        error(pattern, Errors::E_CANNOT_ASSIGN);
        return;
    }

}

void SemanticAnalyzer::registerPattern(const PatternPtr& pattern)
{
    if(pattern->getNodeType() == NodeType::Identifier)
    {
        IdentifierPtr id = std::static_pointer_cast<Identifier>(pattern);
        SymbolPtr s = symbolRegistry->lookupSymbol(id->getIdentifier());
        if(s)
        {
            //already defined
            error(id, Errors::E_DEFINITION_CONFLICT, id->getIdentifier());
        }
        else
        {
            SymbolPlaceHolderPtr pattern(new SymbolPlaceHolder(id->getIdentifier(), id->getType(), SymbolPlaceHolder::R_LOCAL_VARIABLE, 0));
            registerSymbol(pattern);
        }
    }
    else if(pattern->getNodeType() == NodeType::Tuple)
    {
        TuplePtr tuple = std::static_pointer_cast<Tuple>(pattern);
        for(const Tuple::Element& element : tuple->elements)
        {
            registerPattern(element.element);
        }
    }
}

void SemanticAnalyzer::visitComputedProperty(const ComputedPropertyPtr& node)
{

    CodeBlockPtr didSet = node->getDidSet();
    CodeBlockPtr willSet = node->getWillSet();
    CodeBlockPtr getter = node->getGetter();
    CodeBlockPtr setter = node->getSetter();
    TypePtr type = lookupType(node->getDeclaredType());
    assert(type != nullptr);
    node->setType(type);
    //prepare type for getter/setter
    std::vector<Type::Parameter> params;
    TypePtr getterType = Type::newFunction(params, type, nullptr);
    params.push_back(Type::Parameter(type));
    TypePtr setterType = Type::newFunction(params, symbolRegistry->getGlobalScope()->t_Void, false);

    if(getter)
    {
        getter->setType(getterType);
        getter->accept(this);
    }
    if(setter)
    {
        std::wstring name = node->getSetterName().empty() ? L"newValue" : node->getSetterName();
        //TODO: replace the symbol to internal value
        ScopedCodeBlockPtr cb = std::static_pointer_cast<ScopedCodeBlock>(setter);
        cb->setType(setterType);
        cb->getScope()->addSymbol(SymbolPtr(new SymbolPlaceHolder(name, type, SymbolPlaceHolder::R_PARAMETER, SymbolPlaceHolder::F_INITIALIZED)));
        cb->accept(this);
    }
    if(willSet)
    {
        std::wstring setter = node->getWillSetSetter().empty() ? L"newValue" : node->getWillSetSetter();
        //TODO: replace the symbol to internal value
        ScopedCodeBlockPtr cb = std::static_pointer_cast<ScopedCodeBlock>(willSet);
        cb->setType(setterType);
        cb->getScope()->addSymbol(SymbolPtr(new SymbolPlaceHolder(setter, type, SymbolPlaceHolder::R_PARAMETER, SymbolPlaceHolder::F_INITIALIZED)));
        cb->accept(this);
    }
    if(didSet)
    {
        std::wstring setter = node->getDidSetSetter().empty() ? L"oldValue" : node->getDidSetSetter();
        //TODO: replace the symbol to internal value
        ScopedCodeBlockPtr cb = std::static_pointer_cast<ScopedCodeBlock>(didSet);
        cb->setType(setterType);
        cb->getScope()->addSymbol(SymbolPtr(new SymbolPlaceHolder(setter, type, SymbolPlaceHolder::R_PARAMETER, SymbolPlaceHolder::F_INITIALIZED)));
        cb->accept(this);
    }
    //register symbol
    int flags = SymbolPlaceHolder::F_INITIALIZED;
    if(didSet || willSet)
        flags |= SymbolPlaceHolder::F_READABLE | SymbolPlaceHolder::F_WRITABLE;
    if(setter)
        flags |= SymbolPlaceHolder::F_WRITABLE;
    if(getter)
        flags |= SymbolPlaceHolder::F_READABLE;
    SymbolPlaceHolderPtr symbol(new SymbolPlaceHolder(node->getName(), type, SymbolPlaceHolder::R_PROPERTY, flags));
    registerSymbol(symbol);

}
void SemanticAnalyzer::registerSymbol(const SymbolPlaceHolderPtr& symbol)
{
    SymbolScope* scope = symbolRegistry->getCurrentScope();
    NodeType::T nodeType = scope->getOwner()->getNodeType();
    if(nodeType != NodeType::Program)
        symbol->flags |= SymbolPlaceHolder::F_MEMBER;
    scope->addSymbol(symbol);
    switch(nodeType)
    {
        case NodeType::Class:
        case NodeType::Struct:
        case NodeType::Protocol:
        case NodeType::Extension:
        case NodeType::Enum:
            assert(currentType != nullptr);
            static_pointer_cast<TypeBuilder>(currentType)->addMember(symbol->getName(), symbol);
            break;
        default:
            break;
    }
}

void SemanticAnalyzer::visitValueBinding(const ValueBindingPtr& node)
{
    /*if(!node->getInitializer())
    {
        error(node->getInitializer(), Errors::E_LET_REQUIRES_INITIALIZER);
        return;
    }
    */


    //TypePtr type = evaluateType(node->initializer);
    if(IdentifierPtr id = std::dynamic_pointer_cast<Identifier>(node->name))
    {
        TypePtr declaredType = lookupType(node->getDeclaredType());//node->getDeclaredType() == nullptr ? id->getDeclaredType() : node->getDeclaredType());
        StackedValueGuard<TypePtr> guard(t_hint);
        guard.set(declaredType);
        if(!declaredType && !node->initializer)
        {
            error(node, Errors::E_TYPE_ANNOTATION_MISSING_IN_PATTERN);
            return;
        }
        SymbolPtr sym = symbolRegistry->getCurrentScope()->lookup(id->getIdentifier());
        assert(sym != nullptr);
        SymbolPlaceHolderPtr placeholder = std::dynamic_pointer_cast<SymbolPlaceHolder>(sym);
        assert(placeholder != nullptr);
        if(declaredType)
        {
            placeholder->setType(declaredType);
        }
        if(node->initializer)
        {
            node->initializer->accept(this);
            TypePtr actualType = node->initializer->getType();
            assert(actualType != nullptr);
            if(declaredType && !canConvertTo(node->initializer, declaredType))
            {
                error(node, Errors::E_CANNOT_CONVERT_EXPRESSION_TYPE_2, toString(node->initializer), declaredType->toString());
                return;
            }

            if(!declaredType)
                placeholder->setType(actualType);
        }
        assert(placeholder->getType() != nullptr);
    }
    else if(TuplePtr id = std::dynamic_pointer_cast<Tuple>(node->name))
    {
        TypeNodePtr declaredType = id->getDeclaredType();
        if(declaredType)
        {
            checkTupleDefinition(id, node->initializer);
        }
    }
    if(currentType && currentType->getCategory() == Type::Protocol)
    {
        error(node, Errors::E_PROTOCOL_VAR_MUST_BE_COMPUTED_PROPERTY_1);
    }
}


void SemanticAnalyzer::visitValueBindings(const ValueBindingsPtr& node)
{

    if(node->isReadOnly() && currentType && currentType->getCategory() == Type::Protocol)
    {
        error(node, Errors::E_PROTOCOL_CANNOT_DEFINE_LET_CONSTANT_1);
        return;
    }

    for(const ValueBindingPtr& var : *node)
    {
        PatternPtr name = var->getName();
        registerPattern(name);
    }
    int flags = SymbolPlaceHolder::F_INITIALIZING | SymbolPlaceHolder::F_READABLE;
    if(dynamic_cast<TypeDeclaration*>(symbolRegistry->getCurrentScope()->getOwner()))
        flags |= SymbolPlaceHolder::F_MEMBER;
    if(!node->isReadOnly())
        flags |= SymbolPlaceHolder::F_WRITABLE;
    for(const ValueBindingPtr& v : *node)
    {
        PatternPtr name = v->getName();
        ExpressionPtr initializer = v->getInitializer();
        setFlag(name, true, flags);
        v->accept(this);
        setFlag(name, true, SymbolPlaceHolder::F_INITIALIZED);
        setFlag(name, false, SymbolPlaceHolder::F_INITIALIZING);
    }


    //SemanticNodeVisitor::visitValueBindings(node);
}

void SemanticAnalyzer::visitIdentifier(const IdentifierPtr& id)
{
    SymbolPtr sym = NULL;
    SymbolScope* scope = NULL;
    symbolRegistry->lookupSymbol(id->getIdentifier(), &scope, &sym);
    if(!sym)
    {
        compilerResults->add(ErrorLevel::Error, *id->getSourceInfo(), Errors::E_USE_OF_UNRESOLVED_IDENTIFIER_1, id->getIdentifier());
        return;
    }
    if(SymbolPlaceHolderPtr placeholder = std::dynamic_pointer_cast<SymbolPlaceHolder>(sym))
    {
        if((placeholder->flags & SymbolPlaceHolder::F_INITIALIZING))
        {
            error(id, Errors::E_USE_OF_INITIALIZING_VARIABLE, placeholder->getName());
        }
        else if((placeholder->flags & SymbolPlaceHolder::F_INITIALIZED) == 0)
        {
            error(id, Errors::E_USE_OF_UNINITIALIZED_VARIABLE_1, placeholder->getName());
        }
        //check if this identifier is accessed inside a class/protocol/extension/struct/enum but defined not in program
        if(dynamic_cast<TypeDeclaration*>(symbolRegistry->getCurrentScope()->getOwner()))
        {
            if(scope->getOwner()->getNodeType() != NodeType::Program)
            {
                error(id, Errors::E_USE_OF_FUNCTION_LOCAL_INSIDE_TYPE, placeholder->getName());
            }
        }
        id->setType(sym->getType());
    }
    else if(TypePtr type = dynamic_pointer_cast<Type>(sym))
    {
        TypePtr ref = Type::newTypeReference(type);
        id->setType(ref);
    }
}
void SemanticAnalyzer::visitEnumCasePattern(const EnumCasePatternPtr& node)
{
    //TODO: check if the enum case is defined
}

void SemanticAnalyzer::setFlag(const PatternPtr& pattern, bool add, int flag)
{
    NodeType::T t = pattern->getNodeType();
    if(t == NodeType::Identifier)
    {
        IdentifierPtr id = std::static_pointer_cast<Identifier>(pattern);
        SymbolPtr sym = symbolRegistry->getCurrentScope()->lookup(id->getIdentifier());
        assert(sym != nullptr);
        SymbolPlaceHolderPtr placeholder = std::dynamic_pointer_cast<SymbolPlaceHolder>(sym);
        assert(placeholder != nullptr);
        if(add)
            placeholder->flags |= flag;
        else
            placeholder->flags &= ~flag;
    }
    else if(t == NodeType::Tuple)
    {
        TuplePtr tuple = std::static_pointer_cast<Tuple>(pattern);
        for(const Tuple::Element& element : tuple->elements)
        {
            setFlag(element.element, add, flag);
        }
    }

}
