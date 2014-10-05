#include "SymbolResolveAction.h"
#include "SymbolRegistry.h"
#include "ast/ast.h"
#include "common/CompilerResults.h"
#include "swift_errors.h"
#include "ScopedNodes.h"
#include "FunctionSymbol.h"
#include "FunctionOverloadedSymbol.h"
#include "ScopeGuard.h"
#include "ast/NodeSerializer.h"
#include "GenericDefinition.h"
#include <cassert>
#include <set>

USE_SWIFT_NS
using namespace std;
SymbolResolveAction::SymbolResolveAction(SymbolRegistry* symbolRegistry, CompilerResults* compilerResults)
    :SemanticNodeVisitor(symbolRegistry, compilerResults)
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

void SymbolResolveAction::registerPattern(const PatternPtr& pattern)
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

void SymbolResolveAction::visitComputedProperty(const ComputedPropertyPtr& node)
{

    CodeBlockPtr didSet = node->getDidSet();
    CodeBlockPtr willSet = node->getWillSet();
    CodeBlockPtr getter = node->getGetter();
    CodeBlockPtr setter = node->getSetter();
    TypePtr type = lookupType(node->getDeclaredType());
    node->setType(type);
    //prepare type for getter/setter
    std::vector<Type::Parameter> params;
    TypePtr getterType = Type::newFunction(params, type, nullptr);
    params.push_back(Type::Parameter(type));
    TypePtr setterType = Type::newFunction(params, nullptr, false);

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
void SymbolResolveAction::registerSymbol(const SymbolPlaceHolderPtr& symbol)
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
            currentType->addMember(symbol->getName(), symbol);
            break;
        default:
            break;
    }
}
void SymbolResolveAction::visitValueBindings(const ValueBindingsPtr& node)
{
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
        if(initializer)
        {
            initializer->accept(this);
        }
        setFlag(name, true, SymbolPlaceHolder::F_INITIALIZED);
        setFlag(name, false, SymbolPlaceHolder::F_INITIALIZING);
    }
}

void SymbolResolveAction::visitIdentifier(const IdentifierPtr& id)
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

TypePtr SymbolResolveAction::defineType(const std::shared_ptr<TypeDeclaration>& node, Type::Category category)
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
        error(node, Errors::E_INVALID_REDECLARATION_1, id->getName());
        return nullptr;
    }
    //prepare for generic types
    GenericDefinitionPtr generic;
    GenericParametersDefPtr genericParams;
    if(node->getNodeType() == NodeType::Class)
        genericParams = std::static_pointer_cast<ClassDef>(node)->getGenericParametersDef();
    else if(node->getNodeType() == NodeType::Struct)
        genericParams = std::static_pointer_cast<StructDef>(node)->getGenericParametersDef();
    if(genericParams)
    {
        generic = prepareGenericTypes(genericParams);
        generic->registerTo(currentScope);
    }

    //check inheritance clause
    TypePtr parent = nullptr;
    std::vector<TypePtr> protocols;
    bool first = true;

    for(const TypeIdentifierPtr& parentType : node->getParents())
    {
        parentType->accept(this);
        TypePtr ptr = this->lookupType(parentType);
        if(ptr->getCategory() == Type::Class && category == Type::Class)
        {
            if(!first)
            {
                //only the first type can be class type
                std::wstringstream out;
                NodeSerializerW serializer(out);
                parentType->accept(&serializer);
                error(parentType, Errors::E_SUPERCLASS_MUST_APPEAR_FIRST_IN_INHERITANCE_CLAUSE_1, out.str());
                return nullptr;
            }
            first = false;
            parent = ptr;
        }
        else if(ptr->getCategory() == Type::Protocol)
        {
            protocols.push_back(ptr);
        }
        else
        {
            std::wstringstream out;
            NodeSerializerW serializer(out);
            parentType->accept(&serializer);
            if(category == Type::Class)
                error(parentType, Errors::E_INHERITANCE_FROM_NONE_PROTOCOL_NON_CLASS_TYPE_1, out.str());
            else
                error(parentType, Errors::E_INHERITANCE_FROM_NONE_PROTOCOL_TYPE_1, out.str());
            return nullptr;
        }
    }


    //register this type
    type = Type::newType(node->getIdentifier()->getName(), category, node, parent, protocols, generic);
    node->setType(type);
    currentScope->addSymbol(type);

    if(currentType)
        currentType->addMember(type->getName(), type);
    return type;
}

void SymbolResolveAction::visitTypeAlias(const TypeAliasPtr& node)
{
    SymbolScope* scope = nullptr;
    TypePtr type;
    SymbolScope* currentScope = symbolRegistry->getCurrentScope();

    //check if this type is already defined
    symbolRegistry->lookupType(node->getName(), &scope, &type);
    if(type && scope == currentScope)
    {
        //invalid redeclaration of type T
        error(node, Errors::E_INVALID_REDECLARATION_1, node->getName());
        return;
    }
    if(currentType && currentType->getCategory() == Type::Protocol && !node->getType())
    {
        //register a type place holder for protocol
        type = Type::getPlaceHolder();
    }
    else
    {
        type = lookupType(node->getType());
    }
    currentScope->addSymbol(node->getName(), type);
    if(currentType)
    {
        currentType->addMember(node->getName(), type);
    }
}

void SymbolResolveAction::visitClass(const ClassDefPtr& node)
{
    TypePtr type = defineType(node, Type::Class);

    StackedValueGuard<TypePtr> currentType(this->currentType);
    currentType.set(type);

    NodeVisitor::visitClass(node);
}
void SymbolResolveAction::visitStruct(const StructDefPtr& node)
{
    TypePtr type = defineType(node, Type::Struct);
    type->initializer = FunctionOverloadedSymbolPtr(new FunctionOverloadedSymbol());

    StackedValueGuard<TypePtr> currentType(this->currentType);
    currentType.set(type);

    NodeVisitor::visitStruct(node);
    /*
    Rule of initializers for structure:
    1) If no custom initializers, compiler will prepare one or two initializers:
        1.1) A default initializer with no arguments if all let/var fields are defined with a default value
        1.2) A default initializer with all let/var fields as initializer's parameters with the same external name,
            the order of the parameters are the exactly the same as them defined in structure
    2) Compiler will not generate initializers if there's custom initializers
     */
    //TypePtr type = node->getType();
    if(type->getInitializer()->numOverloads() == 0)
    {
        //check all fields if they all have initializer
        bool hasDefaultValues = true;
        for(auto sym : type->getDeclaredStoredProperties())
        {
            if(ValueBindingPtr binding = std::dynamic_pointer_cast<ValueBinding>(sym))
            {
                //TODO: skip computed property
                //TODO: Type only records SymbolPtr
                if(!binding->getInitializer())
                {
                    hasDefaultValues = false;
                    break;
                }
            }
        }
        if(hasDefaultValues)
        {
            //apply rule 1
            std::vector<Type::Parameter> params;
            TypePtr initType = Type::newFunction(params, type, false);
            FunctionSymbolPtr initializer(new FunctionSymbol(node->getIdentifier()->getName(), initType, nullptr));
            type->getInitializer()->add(initializer);
        }

    }


}
void SymbolResolveAction::visitEnum(const EnumDefPtr& node)
{
    defineType(node, Type::Enum);
    NodeVisitor::visitEnum(node);
}
void SymbolResolveAction::visitProtocol(const ProtocolDefPtr& node)
{
    TypePtr type = defineType(node, Type::Protocol);

    StackedValueGuard<TypePtr> currentType(this->currentType);
    currentType.set(type);

    NodeVisitor::visitProtocol(node);
}
void SymbolResolveAction::visitExtension(const ExtensionDefPtr& node)
{
    NodeVisitor::visitExtension(node);

}
void SymbolResolveAction::visitParameter(const ParameterPtr& node)
{
    TypePtr type = lookupType(node->getDeclaredType());
    node->setType(type);
    //check if local name is already defined
    SymbolScope* scope = symbolRegistry->getCurrentScope();
    SymbolPtr sym = scope->lookup(node->getLocalName());
    if(sym)
    {
        error(node, Errors::E_DEFINITION_CONFLICT, node->getLocalName());
        return;
    }
    if(node->getLocalName() == node->getExternalName())
    {
        warning(node, Errors::W_PARAM_CAN_BE_EXPRESSED_MORE_SUCCINCTLY_1, node->getExternalName());
    }
    //TODO: In-out parameters cannot have default values, and variadic parameters cannot be marked as inout. If you mark a parameter as inout, it cannot also be marked as var or let.

    //Protocols use the same syntax as normal methods, but are not allowed to specify default values for method parameters.
    if(node->getDefaultValue())
    {
        error(node, Errors::E_DEFAULT_ARGUMENT_NOT_PERMITTED_IN_A_PROTOCOL_METHOD);
    }


}
void SymbolResolveAction::visitParameters(const ParametersPtr& node)
{
    NodeVisitor::visitParameters(node);
    //check variadic parameter, the last parameter cannot be marked as inout parameter
    if(node->isVariadicParameters())
    {
        ParameterPtr param = node->getParameter(node->numParameters() - 1);
        if(param->isInout())
        {
            error(param, Errors::E_INOUT_ARGUMENTS_CANNOT_BE_VARIADIC);
            return;
        }
    }
    std::set<std::wstring> externalNames;
    //check extraneous shorthand external names
    for(const ParameterPtr& param : *node)
    {
        if(!param->getExternalName().empty())
            externalNames.insert(param->getExternalName());
    }
    for(const ParameterPtr& param : *node)
    {
        if(param->isShorthandExternalName())
        {
            if(externalNames.find(param->getLocalName()) != externalNames.end())
            {
                warning(param, Errors::W_EXTRANEOUS_SHARTP_IN_PARAMETER_1, param->getLocalName());
            }
        }
    }
}



TypePtr SymbolResolveAction::createFunctionType(const std::vector<ParametersPtr>::const_iterator& begin, const std::vector<ParametersPtr>::const_iterator& end, const TypePtr& retType, const GenericDefinitionPtr& generic)
{
    if(begin == end)
        return retType;

    TypePtr returnType = createFunctionType(begin + 1, end, retType, generic);

    std::vector<Type::Parameter> parameterTypes;
    ParametersPtr params = *begin;
    {
        for (const ParameterPtr &param : *params)
        {
            TypePtr paramType = param->getType();
            if(!paramType)
            {
                param->accept(this);
                paramType = param->getType();
                assert(paramType != nullptr);
            }
            assert(paramType != nullptr);
            std::wstring externalName = param->isShorthandExternalName() ? param->getLocalName() : param->getExternalName();
            parameterTypes.push_back(Type::Parameter(externalName, param->isInout(), paramType));
        }
    }

    return Type::newFunction(parameterTypes, returnType, params->isVariadicParameters(), generic);
}
FunctionSymbolPtr SymbolResolveAction::createFunctionSymbol(const FunctionDefPtr& func, const GenericDefinitionPtr& generic)
{
    std::map<std::wstring, TypePtr> genericTypes;



    TypePtr retType = lookupType(func->getReturnType());
    TypePtr funcType = createFunctionType(func->getParametersList().begin(), func->getParametersList().end(), retType, generic);
    FunctionSymbolPtr ret(new FunctionSymbol(func->getName(), funcType, func->getBody()));
    return ret;
}
GenericDefinitionPtr SymbolResolveAction::prepareGenericTypes(const GenericParametersDefPtr& params)
{
    GenericDefinitionPtr ret(new GenericDefinition());
    for (const TypeIdentifierPtr &typeId : *params)
    {
        if (typeId->getNestedType())
        {
            error(typeId->getNestedType(), Errors::E_NESTED_TYPE_IS_NOT_ALLOWED_HERE);
            continue;
        }
        std::wstring name = typeId->getName();
        TypePtr old = ret->get(name);
        if (old != nullptr)
        {
            error(typeId, Errors::E_DEFINITION_CONFLICT);
            continue;
        }
        std::vector<TypePtr> protocols;
        TypePtr type = Type::newType(name, Type::GenericParameter, nullptr, nullptr, protocols);
        ret->add(name, type);
    }
    //add constraint
    for(const GenericConstraintDefPtr& constraint : params->getConstraints())
    {
        //constraint->getConstraintType()
        list<wstring> types;
        TypeIdentifierPtr typeId = constraint->getIdentifier();
        TypePtr type = ret->get(typeId->getName());
        TypePtr expectedType = lookupType(constraint->getExpectedType());

        if(type == nullptr)
        {
            error(typeId, Errors::E_USE_OF_UNDECLARED_TYPE_1, typeId->getName());
            continue;
        }
        if(constraint->getConstraintType() == GenericConstraintDef::EqualsTo)
        {
            //Same-type requirement makes generic parameter 'T' non-generic
            if(typeId->getNestedType() == nullptr)
            {
                error(typeId, Errors::E_SAME_TYPE_REQUIREMENTS_MAKES_GENERIC_PARAMETER_NON_GENERIC_1, typeId->getName());
                continue;
            }
            //check if the reference type is a protocol that contains Self or associated type
            if(expectedType->getCategory() == Type::Protocol)
            {
                if(expectedType->containsSelfType() || expectedType->containsAssociatedType())
                {
                    error(typeId, Errors::E_PROTOCOL_CAN_ONLY_BE_USED_AS_GENERIC_CONSTRAINT_1, expectedType->getName());
                    continue;
                }
            }
        }
        types.push_back(typeId->getName());
        typeId = typeId->getNestedType();
        while(typeId != nullptr)
        {
            wstring name = typeId->getName();
            types.push_back(name);
            TypePtr childType = type->getAssociatedType(name);
            if(!childType)
            {
                //childType = Type::newType(name, Type::Placeholder, nullptr);
                //type->getSymbols()[name] = childType;
                error(typeId, Errors::E_IS_NOT_A_MEMBER_OF_2, name, type->getName());
                return ret;
            }
            type = childType;
            typeId = typeId->getNestedType();
        }
        if(expectedType->getCategory() == Type::Protocol)
        {
            type->addProtocol(expectedType);
        }
        else
        {
            //it's a base type
            if(type->getParentType() != nullptr)
            {
                error(constraint->getIdentifier(), Errors::E_MULTIPLE_INHERITANCE_FROM_CLASS_2_, type->getParentType()->getName(), expectedType->getName());
                continue;
            }
            type->setParentType(expectedType);
        }

        //ret->addConstraint(types);
        GenericDefinition::ConstraintType ct = constraint->getConstraintType() == GenericConstraintDef::DerivedFrom ? GenericDefinition::DerivedFrom : GenericDefinition::EqualsTo;
        ret->addConstraint(types, ct, expectedType);
    }

    return ret;
}
void SymbolResolveAction::visitClosure(const ClosurePtr& node)
{
    ScopedClosurePtr closure = std::static_pointer_cast<ScopedClosure>(node);
    if(node->getCapture())
    {
        node->getCapture()->accept(this);
    }
    node->getParameters()->accept(this);
    prepareParameters(closure->getScope(), node->getParameters());
    for(const StatementPtr& st : *node)
    {
        st->accept(this);
    }


}

void SymbolResolveAction::visitAccessor(const CodeBlockPtr& accessor, const ParametersPtr& params, const SymbolPtr& setter)
{
    if(!accessor)
        return;
    ScopedCodeBlockPtr codeBlock = std::static_pointer_cast<ScopedCodeBlock>(accessor);
    SymbolScope *scope = codeBlock->getScope();
    ScopeGuard scopeGuard(codeBlock.get(), this);
    (void) scopeGuard;

    SymbolPlaceHolderPtr self(new SymbolPlaceHolder(L"self", currentType, SymbolPlaceHolder::R_PARAMETER, SymbolPlaceHolder::F_READABLE | SymbolPlaceHolder::F_INITIALIZED));
    scope->addSymbol(self);
    if(setter)
        scope->addSymbol(setter);

    params->accept(this);
    prepareParameters(scope, params);
    accessor->accept(this);
}
void SymbolResolveAction::visitSubscript(const SubscriptDefPtr &node)
{
    assert(currentType != nullptr);
    //process getter
    ParametersPtr params = node->getParameters();
    visitAccessor(node->getGetter(), params, nullptr);
    wstring setterName = L"newValue";
    if(!node->getSetterName().empty())
        setterName = node->getSetterName();
    SymbolPlaceHolderPtr setter(new SymbolPlaceHolder(setterName, currentType, SymbolPlaceHolder::R_PARAMETER, SymbolPlaceHolder::F_READABLE | SymbolPlaceHolder::F_INITIALIZED));
    visitAccessor(node->getSetter(), params, setter);
    //process setter


    // Register subscript as functions to type
    std::vector<ParametersPtr> paramsList = {params};
    TypePtr retType = lookupType(node->getReturnType());
    if(!node->getGetter() && !node->getSetter())
        return;
    FunctionOverloadedSymbolPtr funcs = currentType->getSubscript();
    if(!funcs)
    {
        funcs = FunctionOverloadedSymbolPtr(new FunctionOverloadedSymbol(L"subscript"));
        currentType->addMember(funcs);
    }

    if(node->getGetter())
    {
        TypePtr funcType = this->createFunctionType(paramsList.begin(), paramsList.end(), retType, nullptr);
        FunctionSymbolPtr func(new FunctionSymbol(L"subscript", funcType, node->getGetter()));
        funcs->add(func);
    }
    if(node->getSetter())
    {
        //TODO: replace nullptr to void
        TypePtr funcType = this->createFunctionType(paramsList.begin(), paramsList.end(), nullptr, nullptr);
        funcType->parameters.push_back(Type::Parameter(L"", false, retType));
        FunctionSymbolPtr func(new FunctionSymbol(L"subscript", funcType, node->getSetter()));
        funcs->add(func);
    }

}
void SymbolResolveAction::visitFunction(const FunctionDefPtr& node)
{
    GenericDefinitionPtr generic;
    if(node->getGenericParametersDef())
        generic = prepareGenericTypes(node->getGenericParametersDef());

    //enter code block's scope
    {
        ScopedCodeBlockPtr codeBlock = std::static_pointer_cast<ScopedCodeBlock>(node->getBody());
        SymbolScope *scope = codeBlock->getScope();

        ScopeGuard scopeGuard(codeBlock.get(), this);
        (void) scopeGuard;

        if(generic)
            generic->registerTo(scope);
        if(currentType && (node->getSpecifiers() & (TypeSpecifier::Class | TypeSpecifier::Static)) == 0)
        {
            SymbolPlaceHolderPtr self(new SymbolPlaceHolder(L"self", currentType, SymbolPlaceHolder::R_PARAMETER, SymbolPlaceHolder::F_READABLE | SymbolPlaceHolder::F_INITIALIZED));
            scope->addSymbol(self);
        }


        for (const ParametersPtr &params : node->getParametersList())
        {
            params->accept(this);
            prepareParameters(scope, params);
        }
        node->getBody()->accept(this);
        lookupType(node->getReturnType());
    }


    FunctionSymbolPtr func = createFunctionSymbol(node, generic);
    node->setType(func->getType());
    node->getBody()->setType(func->getType());
    //check if the same symbol has been defined in this scope
    SymbolScope* scope = symbolRegistry->getCurrentScope();
    SymbolPtr oldSym = scope->lookup(func->getName());
    SymbolPtr sym;
    if(oldSym)
    {
        FunctionOverloadedSymbolPtr overload = std::dynamic_pointer_cast<FunctionOverloadedSymbol>(oldSym);
        if(!overload)
        {
            //wrap it as overload
            if(FunctionSymbolPtr oldFunc = std::dynamic_pointer_cast<FunctionSymbol>(oldSym))
            {
                overload = FunctionOverloadedSymbolPtr(new FunctionOverloadedSymbol(func->getName()));
                overload->add(oldFunc);
                scope->removeSymbol(oldSym);
                scope->addSymbol(overload);
            }
            else
            {
                //error, symbol with same name exists
                error(node, Errors::E_INVALID_REDECLARATION_1, node->getName());
                abort();
            }
        }
        overload->add(func);
        sym = overload;
    }
    else
    {
        scope->addSymbol(func);
        sym = func;
    }
    //put it into type's SymbolMap
    if(TypeDeclaration* declaration = dynamic_cast<TypeDeclaration*>(symbolRegistry->getCurrentScope()->getOwner()))
    {
        if(declaration->getType())
        {
            declaration->getType()->addMember(sym->getName(), sym);
        }
    }




}

void SymbolResolveAction::visitDeinit(const DeinitializerDefPtr& node)
{
    SemanticNodeVisitor::visitDeinit(node);
}

void SymbolResolveAction::prepareParameters(SymbolScope* scope, const ParametersPtr& params)
{
    //check and prepare for parameters

    for(const ParameterPtr& param : *params)
    {
        assert(param->getType() != nullptr);
        SymbolPtr sym = scope->lookup(param->getLocalName());
        if(sym)
        {
            error(param, Errors::E_DEFINITION_CONFLICT, param->getLocalName());
            return;
        }
        sym = SymbolPtr(new SymbolPlaceHolder(param->getLocalName(), param->getType(), SymbolPlaceHolder::R_PARAMETER, SymbolPlaceHolder::F_INITIALIZED));
        scope->addSymbol(sym);
    }
    //prepare for implicit parameter self

}
void SymbolResolveAction::visitInit(const InitializerDefPtr& node)
{
    TypeDeclaration* declaration = dynamic_cast<TypeDeclaration*>(symbolRegistry->getCurrentScope()->getOwner());

    assert(declaration != nullptr);
    TypePtr type = declaration->getType();

    node->getParameters()->accept(this);
    ScopedCodeBlockPtr body = std::static_pointer_cast<ScopedCodeBlock>(node->getBody());
    prepareParameters(body->getScope(), node->getParameters());
    //prepare implicit self
    SymbolPlaceHolderPtr self(new SymbolPlaceHolder(L"self", currentType, SymbolPlaceHolder::R_PARAMETER, SymbolPlaceHolder::F_READABLE | SymbolPlaceHolder::F_INITIALIZED));
    body->getScope()->addSymbol(self);

    std::vector<Type::Parameter> params;
    for(const ParameterPtr& param : *node->getParameters())
    {
        const std::wstring& externalName = param->isShorthandExternalName() ? param->getLocalName() : param->getExternalName();
        params.push_back(Type::Parameter(externalName, param->isInout(), param->getType()));
    }

    TypePtr funcType = Type::newFunction(params, type, node->getParameters()->isVariadicParameters());


    FunctionSymbolPtr init(new FunctionSymbol(type->getName(), funcType, nullptr));
    type->getInitializer()->add(init);



    node->getBody()->accept(this);
}


