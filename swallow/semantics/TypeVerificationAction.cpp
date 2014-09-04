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
void TypeVerificationAction::visitConstants(const ConstantsPtr& node)
{
    if(currentType && currentType->getCategory() == Type::Protocol)
    {
        error(node, Errors::E_PROTOCOL_CANNOT_DEFINE_LET_CONSTANT);
    }
}
void TypeVerificationAction::visitVariable(const VariablePtr& node)
{
    if(currentType && currentType->getCategory() == Type::Protocol)
    {
        if (!node->getGetter() && !node->getSetter() && !node->getWillSet() && !node->getDidSet())
        {
            error(node, Errors::E_PROTOCOL_VAR_MUST_BE_COMPUTED_PROPERTY);
        }
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
        verifyProtocolConform(type, protocol);
    }
}
void TypeVerificationAction::verifyProtocolConform(const TypePtr& type, const TypePtr& protocol)
{
    for(auto entry : protocol->getSymbols())
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
        else if(requirement == Type::getPlaceHolder())
        {
            //verify inner type
            SymbolPtr sym = type->getSymbols()[entry.first];
            if(!(std::dynamic_pointer_cast<Type>(sym)))
            {
                error(type->getReference(), Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_TYPE, entry.first);
            }
        }
        else if(TypePtr type = std::dynamic_pointer_cast<Type>(requirement))
        {
            //type can be ignored
        }
        else
        {
            //verify computed properties
        }
    }
}
void TypeVerificationAction::verifyProtocolFunction(const TypePtr& type, const TypePtr& protocol, const FunctionSymbolPtr& expected)
{
    SymbolPtr sym = type->getSymbols()[expected->getName()];
    TypePtr expectedType = expected->getType();
    assert(expectedType != nullptr);
    if(!sym)
    {
        error(type->getReference(), Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_FUNCTION, expected->getName());
        return;
    }
    if(const FunctionSymbolPtr& func = std::dynamic_pointer_cast<FunctionSymbol>(sym))
    {
        //verify if they're the same type
        TypePtr funcType = func->getType();
        assert(funcType != nullptr);
        if(*funcType != *expectedType)
        {
            error(type->getReference(),  Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_FUNCTION, expected->getName());
        }
        return;
    }
    if(FunctionOverloadedSymbolPtr funcs = std::dynamic_pointer_cast<FunctionOverloadedSymbol>(sym))
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
            error(type->getReference(), Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_FUNCTION, expected->getName());
        return;
    }
    error(type->getReference(), Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_FUNCTION, expected->getName());
}
