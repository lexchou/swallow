#include "SemanticNodeVisitor.h"
#include "swift_types.h"
#include "common/CompilerResults.h"
#include "ast/Node.h"
#include "SymbolRegistry.h"
#include "Type.h"
#include "ast/TypeNode.h"
#include "ast/TypeIdentifier.h"
#include "ast/TupleType.h"
#include "ast/FunctionType.h"
#include "ast/ArrayType.h"
#include <cassert>
#include "swift_errors.h"
#include "ast/NodeSerializer.h"
#include "GlobalScope.h"
#include "ast/OptionalType.h"
#include "GenericDefinition.h"

USE_SWIFT_NS


/**
* Abort the visitor
*/
void SemanticNodeVisitor::abort()
{
    throw Abort();
}

/**
 * Outputs an compiler error
 */
void SemanticNodeVisitor::error(const NodePtr& node, int code, const std::wstring& item)
{
    compilerResults->add(ErrorLevel::Error, *node->getSourceInfo(), code, item);
    abort();
}
void SemanticNodeVisitor::warning(const NodePtr& node, int code, const std::wstring& item)
{
    compilerResults->add(ErrorLevel::Warning, *node->getSourceInfo(), code, item);
}

std::wstring SemanticNodeVisitor::toString(const NodePtr& node)
{
    std::wstringstream out;
    NodeSerializerW serializer(out);
    node->accept(&serializer);
    return out.str();
}
TypePtr SemanticNodeVisitor::lookupType(const TypeNodePtr& type)
{
    if(!type)
        return nullptr;
    if(TypeIdentifierPtr id = std::dynamic_pointer_cast<TypeIdentifier>(type))
    {
        //TODO: make a generic type if possible
        TypePtr ret = symbolRegistry->lookupType(id->getName());
        if(!ret)
        {
            std::wstring str = toString(type);
            error(type, Errors::E_USE_OF_UNDECLARED_TYPE, str);
            abort();
        }
        GenericDefinitionPtr generic = ret->getGenericDefinition();
        if(generic == nullptr && id->numGenericArguments() == 0)
            return ret;
        if(generic == nullptr && id->numGenericArguments() > 0)
        {
            std::wstring str = toString(type);
            error(id, Errors::E_CANNOT_SPECIALIZE_NON_GENERIC_TYPE, str);
            return nullptr;
        }
        if(generic != nullptr && id->numGenericArguments() == 0)
        {
            std::wstring str = toString(type);
            error(id, Errors::E_GENERIC_TYPE_ARGUMENT_REQUIRED, str);
            return nullptr;
        }
        if(id->numGenericArguments() > generic->numParameters())
        {
            std::wstring str = toString(type);
            error(id, Errors::E_GENERIC_TYPE_SPECIALIZED_WITH_TOO_MANY_TYPE_PARAMETERS, str);
            return nullptr;
        }
        if(id->numGenericArguments() < generic->numParameters())
        {
            std::wstring str = toString(type);
            error(id, Errors::E_GENERIC_TYPE_SPECIALIZED_WITH_INSUFFICIENT_TYPE_PARAMETERS, str);
            return nullptr;
        }
        //check type
        std::vector<TypePtr> typeArguments;
        for(auto arg : *id)
        {
            TypePtr argType = lookupType(arg);
            if(!argType)
                return nullptr;
            typeArguments.push_back(argType);
        }
        ret = Type::newSpecializedType(ret, typeArguments);
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
        GlobalScope* scope = symbolRegistry->getGlobalScope();
        TypePtr Array = scope->t_Array;
        TypePtr innerType = lookupType(array->getInnerType());
        assert(innerType != nullptr);
        assert(Array != nullptr);
        std::vector<TypePtr> typeArguments = {innerType};
        TypePtr ret = Type::newSpecializedType(Array, typeArguments);
        return ret;
    }
    if(OptionalTypePtr opt = std::dynamic_pointer_cast<OptionalType>(type))
    {
        GlobalScope* scope = symbolRegistry->getGlobalScope();
        TypePtr Optional = scope->t_Optional;
        TypePtr innerType = lookupType(opt->getInnerType());
        assert(innerType != nullptr);
        assert(Optional != nullptr);
        std::vector<TypePtr> typeArguments = {innerType};
        TypePtr ret = Type::newSpecializedType(Optional, typeArguments);
        return ret;
    }
    if(FunctionTypePtr func = std::dynamic_pointer_cast<FunctionType>(type))
    {
        TypePtr retType = nullptr;
        if(func->getReturnType())
        {
            retType = lookupType(func->getReturnType());
        }
    }
    assert(0 && "Unsupported type");
    return nullptr;
}
