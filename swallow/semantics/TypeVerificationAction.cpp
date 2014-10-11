#include "TypeVerificationAction.h"
#include "ast/ast.h"
#include "FunctionSymbol.h"
#include "FunctionOverloadedSymbol.h"
#include "swift_errors.h"
#include <cassert>
USE_SWIFT_NS

TypeVerificationAction::TypeVerificationAction(SymbolRegistry* symbolRegistry, CompilerResults* compilerResults)
:SemanticNodeVisitor(symbolRegistry, compilerResults)
{
}
void TypeVerificationAction::visitValueBindings(const ValueBindingsPtr& node)
{
    if(node->isReadOnly() && currentType && currentType->getCategory() == Type::Protocol)
    {
        error(node, Errors::E_PROTOCOL_CANNOT_DEFINE_LET_CONSTANT_1);
        return;
    }
    SemanticNodeVisitor::visitValueBindings(node);
}
void TypeVerificationAction::visitValueBinding(const ValueBindingPtr &node)
{
    if(currentType && currentType->getCategory() == Type::Protocol)
    {
        error(node, Errors::E_PROTOCOL_VAR_MUST_BE_COMPUTED_PROPERTY_1);
    }
}
void TypeVerificationAction::visitClass(const ClassDefPtr& node)
{
    const TypePtr& type = node->getType();
    assert(type != nullptr);
    verifyProtocolConform(type);
    StackedValueGuard<TypePtr> currentType(this->currentType);
    currentType.set(type);
    SemanticNodeVisitor::visitClass(node);
}

void TypeVerificationAction::visitStruct(const StructDefPtr& node)
{
    const TypePtr& type = node->getType();
    assert(type != nullptr);
    verifyProtocolConform(type);
    StackedValueGuard<TypePtr> currentType(this->currentType);
    currentType.set(type);
    SemanticNodeVisitor::visitStruct(node);
}
void TypeVerificationAction::visitEnum(const EnumDefPtr& node)
{
    //StackedValueGuard<TypePtr> currentType(this->currentType);
    //currentType.set(type);
    SemanticNodeVisitor::visitEnum(node);
}
void TypeVerificationAction::visitExtension(const ExtensionDefPtr& node)
{
    SemanticNodeVisitor::visitExtension(node);
}
void TypeVerificationAction::visitProtocol(const ProtocolDefPtr& node)
{
    StackedValueGuard<TypePtr> currentType(this->currentType);
    currentType.set(node->getType());
    SemanticNodeVisitor::visitProtocol(node);
}

/**
 * Verify if the specified type conform to the given protocol
 */
void TypeVerificationAction::verifyProtocolConform(const TypePtr& type)
{
    for(const TypePtr& protocol : type->getProtocols())
    {
        if(protocol->containsAssociatedType())
            continue;//it's already done in SymbolResolveAction.cpp
        verifyProtocolConform(type, protocol);
    }
}
void TypeVerificationAction::verifyProtocolConform(const TypePtr& type, const TypePtr& protocol)
{
    for(auto entry : protocol->getDeclaredMembers())
    {
        SymbolPtr requirement = entry.second;
        if(FunctionOverloadedSymbolPtr funcs = std::dynamic_pointer_cast<FunctionOverloadedSymbol>(requirement))
        {
            //verify function
            for(auto func : *funcs)
            {
                verifyProtocolFunction(type, protocol, func);
            }
        }
        else if(FunctionSymbolPtr func = std::dynamic_pointer_cast<FunctionSymbol>(requirement))
        {
            //verify function
            verifyProtocolFunction(type, protocol, func);
        }
        /*
        else if(requirement == Type::getPlaceHolder())
        {
            //verify inner type
            SymbolPtr sym = type->getAssociatedType(entry.first);
            if(!(std::dynamic_pointer_cast<Type>(sym)))
            {
                //Type %0 does not conform to protocol %1, unimplemented type %2
                error(type->getReference(), Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_TYPE_3, type->getName(), protocol->getName(), entry.first);
            }
        }*/
        else if(TypePtr t = std::dynamic_pointer_cast<Type>(requirement))
        {
            //type can be ignored
        }
        else if(SymbolPlaceHolderPtr prop = std::dynamic_pointer_cast<SymbolPlaceHolder>(requirement))
        {
            //verify computed properties
            assert(prop->flags & SymbolPlaceHolder::F_MEMBER && prop->getRole() == SymbolPlaceHolder::R_PROPERTY);
            SymbolPtr sym = type->getMember(entry.first);
            SymbolPlaceHolderPtr sp = std::dynamic_pointer_cast<SymbolPlaceHolder>(sym);
            if(!sp)
            {
                error(type->getReference(), Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_PROPERTY_3, type->getName(), protocol->getName(), entry.first);
            }
            bool expectedSetter = prop->flags & SymbolPlaceHolder::F_WRITABLE;
            bool actualSetter = sp->flags & SymbolPlaceHolder::F_WRITABLE;
            if(expectedSetter && !actualSetter)
            {
                error(type->getReference(), Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNWRITABLE_PROPERTY_3, type->getName(), protocol->getName(), entry.first);
            }
        }
    }
}
void TypeVerificationAction::verifyProtocolFunction(const TypePtr& type, const TypePtr& protocol, const FunctionSymbolPtr& expected)
{
    SymbolPtr sym = type->getMember(expected->getName());
    TypePtr expectedType = expected->getType();
    assert(expectedType != nullptr);
    if(!sym)
    {
        error(type->getReference(), Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_FUNCTION_3, type->getName(), protocol->getName(), expected->getName());
        return;
    }
    else if(const FunctionSymbolPtr& func = std::dynamic_pointer_cast<FunctionSymbol>(sym))
    {
        //verify if they're the same type
        TypePtr funcType = func->getType();
        assert(funcType != nullptr);
        if(*funcType != *expectedType)
        {
            error(type->getReference(),  Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_FUNCTION_3, type->getName(), protocol->getName(), expected->getName());
        }
        return;
    }
    else if(FunctionOverloadedSymbolPtr funcs = std::dynamic_pointer_cast<FunctionOverloadedSymbol>(sym))
    {
        //verify if they're the same type
        bool found = false;
        for(const FunctionSymbolPtr& func : *funcs)
        {
            if(*func->getType() != *expectedType)
            {
                found = true;
                break;
            }
        }
        if(!found)
            error(type->getReference(), Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_FUNCTION_3, type->getName(), protocol->getName(), expected->getName());
        return;
    }
    else if(SymbolPlaceHolderPtr prop = std::dynamic_pointer_cast<SymbolPlaceHolder>(sym))
    {


    }
    error(type->getReference(), Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_FUNCTION_3, type->getName(), protocol->getName(), expected->getName());
}
