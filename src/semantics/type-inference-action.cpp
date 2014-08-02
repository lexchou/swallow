#include <semantics/symbol-registry.h>
#include <common/compiler_results.h>
#include <swift_errors.h>
#include "type-inference-action.h"
#include "ast/variables.h"
#include "ast/constant.h"
#include "ast/identifier.h"
#include "ast/tuple.h"
#include "scoped-nodes.h"
#include "ast/type-node.h"
#include "type.h"
#include "semantic-types.h"
#include "ast/type-identifier.h"
#include "ast/tuple-type.h"
#include "ast/array-type.h"
#include "ast/function-type.h"
USE_SWIFT_NS


TypeInferenceAction::TypeInferenceAction(SymbolRegistry* symbolRegistry, CompilerResults* compilerResults)
    :SemanticNodeVisitor(symbolRegistry, compilerResults)
{

}

TypePtr TypeInferenceAction::lookupType(const TypeNodePtr& type)
{
    if(TypeIdentifierPtr id = std::dynamic_pointer_cast<TypeIdentifier>(type))
    {
        //TODO: make a generic type if possible
        TypePtr ret = symbolRegistry->lookupType(id->getName());
        if(!ret)
        {
            std::wstringstream out;
            type->serialize(out);
            compilerResults->add(ErrorLevel::Error, *type->getSourceInfo(), Errors::E_USE_OF_UNDECLARED_TYPE, out.str());
            abort();
        }
        return ret;
    }
    if(TupleTypePtr tuple = std::dynamic_pointer_cast<TupleType>(type))
    {
        std::vector<TypePtr> elementTypes;
        for(const TupleType::TupleElement& e : *tuple)
        {
            TypePtr t = lookupType(e.type);
            elementTypes.push_back(t);
        }
        return Type::newTuple(elementTypes);
    }
    if(ArrayTypePtr array = std::dynamic_pointer_cast<ArrayType>(type))
    {
        TypePtr innerType = lookupType(array->getInnerType());
        return Type::newArrayType(innerType);
    }
    if(FunctionTypePtr func = std::dynamic_pointer_cast<FunctionType>(type))
    {
        TypePtr retType = nullptr;
        if(func->getReturnType())
        {
            retType = lookupType(func->getReturnType());
        }
    }
    return nullptr;
}


void TypeInferenceAction::checkTupleDefinition(const TuplePtr& tuple, const ExpressionPtr& initializer)
{
    //this is a tuple definition, the corresponding declared type must be a tuple type
    TypeNodePtr declaredType = tuple->getDeclaredType();
    TypePtr type = lookupType(declaredType);
    if(!type)
    {
        std::wstringstream out;
        declaredType->serialize(out);
        compilerResults->add(ErrorLevel::Error, *tuple->getSourceInfo(), Errors::E_USE_OF_UNDECLARED_TYPE, out.str());
        return;
    }
    if(!(type->isTuple()))
    {
        //tuple definition must have a tuple type definition
        std::wstringstream out;
        declaredType->serialize(out);
        compilerResults->add(ErrorLevel::Error, *tuple->getSourceInfo(), Errors::E_TUPLE_PATTERN_MUST_MATCH_TUPLE_TYPE, out.str());
        return;
    }
    if(tuple->numElements() != type->numElementTypes())
    {
        //tuple pattern has the wrong length for tuple type '%'
        std::wstringstream out;
        declaredType->serialize(out);
        compilerResults->add(ErrorLevel::Error, *tuple->getSourceInfo(), Errors::E_TUPLE_PATTERN_MUST_MATCH_TUPLE_TYPE, out.str());
        return;
    }
    //check if initializer has the same type with the declared type
    if(initializer)
    {
        TypePtr valueType = evaluateType(initializer);
        if(valueType && *valueType != *type)
        {
            //tuple pattern has the wrong length for tuple type '%'
            std::wstringstream out;
            declaredType->serialize(out);
            compilerResults->add(ErrorLevel::Error, *initializer->getSourceInfo(), Errors::E_TUPLE_TYPES_HAVE_A_DIFFERENT_NUMBER_OF_ELEMENT, out.str());
            return;
        }
    }


    for(const PatternPtr& p : *tuple)
    {
        NodeType::T nodeType = p->getNodeType();
        if(nodeType != NodeType::Identifier)
        {

        }

    }
}

void TypeInferenceAction::visitVariable(const VariablePtr& node)
{
}

void TypeInferenceAction::visitConstant(const ConstantPtr& node)
{
    TypePtr type = evaluateType(node->initializer);
    if(IdentifierPtr id = std::dynamic_pointer_cast<Identifier>(node->name))
    {
        symbolRegistry->getCurrentScope()->addSymbol(std::static_pointer_cast<SymboledConstant>(node));
        if(id->getDeclaredType() == NULL)
        {
            id->setType(type);
        }
        else
        {
            //check if the type is convertible

        }
    }
    else if(TuplePtr id = std::dynamic_pointer_cast<Tuple>(node->name))
    {
        TypeNodePtr declaredType = id->getDeclaredType();
        if(declaredType)
        {
            checkTupleDefinition(id, node->initializer);
        }
    }
}
TypePtr TypeInferenceAction::evaluateType(const ExpressionPtr& expr)
{
    switch(expr->getNodeType())
    {
        case NodeType::IntegerLiteral:
            return symbolRegistry->lookupType(L"Int");
        case NodeType::FloatLiteral:
            return symbolRegistry->lookupType(L"Float");
        case NodeType::StringLiteral:
            return symbolRegistry->lookupType(L"String");
        default:
            return nullptr;
    }
}