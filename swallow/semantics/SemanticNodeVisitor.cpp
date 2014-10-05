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
#include "GenericArgument.h"
#include <sstream>

USE_SWIFT_NS


/**
* Abort the visitor
*/
void SemanticNodeVisitor::abort()
{
    throw Abort();
}
void SemanticNodeVisitor::error(const NodePtr& node, int code, const std::vector<std::wstring>& items)
{
    compilerResults->add(ErrorLevel::Error, *node->getSourceInfo(), code, items);
    abort();
}
/**
 * Outputs an compiler error
 */
void SemanticNodeVisitor::error(const NodePtr& node, int code, const std::wstring& item)
{
    ResultItems items = {item};
    error(node, code, items);
}
void SemanticNodeVisitor::error(const NodePtr& node, int code, const std::wstring& item1, const std::wstring& item2)
{
    ResultItems items = {item1, item2};
    error(node, code, items);
}
void SemanticNodeVisitor::error(const NodePtr& node, int code, const std::wstring& item1, const std::wstring& item2, const std::wstring& item3)
{
    ResultItems items = {item1, item2, item3};
    error(node, code, items);
}
void SemanticNodeVisitor::error(const NodePtr& node, int code, const std::wstring& item1, const std::wstring& item2, const std::wstring& item3, const std::wstring& item4)
{
    ResultItems items = {item1, item2, item3, item4};
    error(node, code, items);
}

void SemanticNodeVisitor::error(const NodePtr& node, int code)
{
    error(node, code, L"");
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
std::wstring SemanticNodeVisitor::toString(int i)
{
    std::wstringstream s;
    s<<i;
    return s.str();
}
TypePtr SemanticNodeVisitor::lookupType(const TypeNodePtr& type)
{
    if(!type)
        return nullptr;
    TypePtr ret = type->getType();
    if(!ret)
    {
        ret = lookupTypeImpl(type);
        type->setType(ret);
    }
    return ret;
}
TypePtr SemanticNodeVisitor::lookupTypeImpl(const TypeNodePtr &type)
{
    if(TypeIdentifierPtr id = std::dynamic_pointer_cast<TypeIdentifier>(type))
    {
        //TODO: make a generic type if possible
        TypePtr ret = symbolRegistry->lookupType(id->getName());
        if(!ret)
        {
            std::wstring str = toString(type);
            error(type, Errors::E_USE_OF_UNDECLARED_TYPE_1, str);
            abort();
        }
        GenericDefinitionPtr generic = ret->getGenericDefinition();
        if(generic == nullptr && id->numGenericArguments() == 0)
            return ret;
        if(generic == nullptr && id->numGenericArguments() > 0)
        {
            std::wstring str = toString(type);
            error(id, Errors::E_CANNOT_SPECIALIZE_NON_GENERIC_TYPE_1, str);
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
            std::wstring got = toString(id->numGenericArguments());
            std::wstring expected = toString(generic->numParameters());
            error(id, Errors::E_GENERIC_TYPE_SPECIALIZED_WITH_TOO_MANY_TYPE_PARAMETERS_3, str, got, expected);
            return nullptr;
        }
        if(id->numGenericArguments() < generic->numParameters())
        {
            std::wstring str = toString(type);
            std::wstring got = toString(id->numGenericArguments());
            std::wstring expected = toString(generic->numParameters());
            error(id, Errors::E_GENERIC_TYPE_SPECIALIZED_WITH_INSUFFICIENT_TYPE_PARAMETERS_3, str, got, expected);
            return nullptr;
        }
        //check type
        GenericArgumentPtr genericArgument(new GenericArgument(generic));
        for(auto arg : *id)
        {
            TypePtr argType = lookupType(arg);
            if(!argType)
                return nullptr;
            genericArgument->add(argType);
        }
        ret = Type::newSpecializedType(ret, genericArgument);
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
        TypePtr ret = Type::newSpecializedType(Array, innerType);
        return ret;
    }
    if(OptionalTypePtr opt = std::dynamic_pointer_cast<OptionalType>(type))
    {
        GlobalScope* scope = symbolRegistry->getGlobalScope();
        TypePtr Optional = scope->t_Optional;
        TypePtr innerType = lookupType(opt->getInnerType());
        assert(innerType != nullptr);
        assert(Optional != nullptr);
        TypePtr ret = Type::newSpecializedType(Optional, innerType);
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
